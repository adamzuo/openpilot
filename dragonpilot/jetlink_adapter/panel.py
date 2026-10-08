"""Full Jetlink settings and device-specific tici/mici HUD indicators."""
import math
import time


def settings_items(params):
  from openpilot.system.ui.widgets.list_view import text_spin_button_item, toggle_item, text_item
  from openpilot.selfdrive.ui.ui_state import ui_state
  def put(key, value):
    if ui_state.is_offroad():
      params.put(key, value)
  mode = params.get('JetlinkLink') or 0
  return [
    text_spin_button_item('Jetlink 連線', options=['關閉', 'USB / Android', 'iOS'], initial_index=mode,
      callback=lambda i: put('JetlinkLink', i), enabled=ui_state.is_offroad,
      description='啟用後使用完整外部模型；連線或效能不足時使用本機模型。設定於下次行車生效。'),
    toggle_item('為 iPhone 充電', initial_state=params.get_bool('JetlinkChargePhone'),
      callback=lambda x: put('JetlinkChargePhone', x), enabled=ui_state.is_offroad),
    *[text_item(title, lambda key=key: settings_snapshot()[key] if key in ('connection', 'loading') else '',
                description=lambda key=key: settings_snapshot()[key] if key not in ('connection', 'loading') else '')
      for key, title in STATUS_ROWS],
  ]


STATUS_ROWS = (('connection', 'Jetlink 連線狀態'), ('model', 'eGPU 目標模型'),
               ('active', 'eGPU 已備妥模型'), ('loading', 'eGPU 載入進度'),
               ('message', '載入詳細資訊'), ('reason', '連線異常'))


def status_fields(link, enabled=True):
  """Shared view of upstream Status; a built model is not proof it is driving."""
  get = lambda key, default=None: getattr(link, key, default)
  connection = 'OFF' if not enabled else 'UNKNOWN' if link is None else 'ERROR' if get('reason') else 'CONNECTED' if get('present') else 'OFFLINE'
  progress = get('progress') or {}
  stage = str(progress.get('stage') or '')
  frac = progress.get('frac')
  percent = ''
  if stage in ('download', 'upload', 'build', 'compile') and isinstance(frac, (int, float)) and math.isfinite(frac):
    percent = f' {max(0, min(100, round(frac*100)))}%'
  loading = (stage + percent) if enabled and stage and stage != 'ready' else 'READY' if enabled and get('runnable', get('ready', False)) else 'OFF' if not enabled else 'WAIT'
  message = str(progress.get('msg') or '—') if enabled else '—'
  if progress.get('drops') and enabled:
    message += f" · {progress['drops']} drops; check cable/app"
  return dict(connection=connection, model=get('model') or '—', active=get('active_model') or '—',
              loading=loading, message=message, reason=get('reason') or '—',
              transport=get('transport') or '', percent=percent)


def settings_snapshot():
  from dragonpilot.jetlink_adapter import status
  from openpilot.common.params import Params
  now = time.monotonic()
  if now - getattr(settings_snapshot, 'checked', -10) > .5:
    enabled = bool(Params().get('JetlinkLink'))
    link = status() if enabled else None
    fields = status_fields(link, enabled)
    fields['connection'] += (' · ' + fields['transport']) if fields['transport'] else ''
    settings_snapshot.fields = fields
    settings_snapshot.checked = now
  return settings_snapshot.fields


def mici_settings_items(params):
  from openpilot.selfdrive.ui.mici.widgets.button import BigButton, BigMultiToggle, BigToggle
  from openpilot.selfdrive.ui.ui_state import ui_state
  class LiveStatus(BigButton):
    def __init__(self, key, title):
      super().__init__('', title, scroll=True)
      self.key = key
    def _update_state(self):
      super()._update_state()
      value = settings_snapshot()[self.key]
      if self.text != value:
        self.set_text(value)
  options = ['OFF', 'USB / Android', 'iOS']
  mode = BigMultiToggle('Jetlink', options, select_callback=lambda value: params.put('JetlinkLink', options.index(value)) if ui_state.is_offroad() else None)
  mode.set_value(options[int(params.get('JetlinkLink') or 0)])
  mode.set_enabled(ui_state.is_offroad)
  charge = BigToggle('charge iPhone', initial_state=params.get_bool('JetlinkChargePhone'),
                     toggle_callback=lambda value: params.put('JetlinkChargePhone', value) if ui_state.is_offroad() else None)
  charge.set_enabled(ui_state.is_offroad)
  return [mode, charge, *[LiveStatus(key, title) for key, title in STATUS_ROWS]]


def indicator(state, proving, fresh, link):
  """Return icon kind, full label, short label; cached engine readiness isn't activity."""
  if not fresh:
    return 'failed', '模型狀態未更新', 'NO DATA'
  if state == 'full: running':
    return ('loading', '完整模型準備中', 'CHECK') if proving else ('active', '完整模型運作', 'ACTIVE')
  if link is not None and (link.reason or not link.enabled):
    return 'failed', '無法使用・本機模型', 'ERROR'
  if state == 'full: unavailable':
    return 'failed', '連線失效・本機模型', 'LOCAL'
  if link is not None and not link.present:
    return 'failed', '未連線・本機模型', 'OFFLINE'
  if state == 'full: ready':
    return 'ready', '已就緒・等待切換', 'READY'
  if state in ('full: joining', 'full: retrying'):
    return 'loading', '連線／重連中', 'WAIT'
  # modeld refused prepare()/attach(): never pretend a cached ready engine is driving.
  return 'failed', '本機模型', 'LOCAL'


def geometry(rect, variant):
  """Logical pixels, after the GUI's display scaling. Keeps DP overlays clear."""
  if variant == 'mici':
    # 536x240 screen; camera viewport is 476x240 after its 60-pixel side panel.
    # Between centered lead and the rightmost steering icon, above rounded border.
    return rect.x + rect.width - 180, rect.y + rect.height - 62, 112, 38
  # Above the 78-pixel performance bar, centered clear of either left/right DM icon.
  width = min(500, rect.width - 80)
  return rect.x + (rect.width - width)/2, rect.y + rect.height - 152, width, 56


def draw_status(rect, variant='tici', show=True):
  if not show:
    return
  import pyray as rl
  from openpilot.selfdrive.ui.ui_state import ui_state
  from openpilot.system.ui.lib.application import gui_app, FontWeight
  from openpilot.common.params import Params
  from openpilot.system.ui.lib.text_measure import measure_text_cached
  from dragonpilot.jetlink_adapter import status
  now = time.monotonic()
  if now-getattr(draw_status, 'checked', -10) > 1:
    draw_status.checked = now
    draw_status.enabled = bool(Params().get('JetlinkLink'))
    draw_status.link = status() if draw_status.enabled else None
  if not getattr(draw_status, 'enabled', False):
    return
  sm = ui_state.sm
  fresh = (sm.alive['modelExt'] and sm.valid['modelExt'] and
           sm.recv_frame['modelExt'] >= ui_state.started_frame)
  state = sm['modelExt'].jetlinkState if fresh else ''
  proving = sm['modelExt'].jetlinkProving if fresh else False
  kind, label, short = indicator(state, proving, fresh, draw_status.link)
  name = {'active': 'chestnut_green.png', 'failed': 'chestnut_orange.png',
          'loading': 'chestnut.png', 'ready': 'chestnut.png'}[kind]
  compact = variant == 'mici'
  icon_w = (46 if kind == 'failed' else 38) if compact else (75 if kind == 'failed' else 60)
  icon_h = 28 if compact else 44
  texture = gui_app.texture('icons_mici/'+name, icon_w, icon_h)
  alpha = int(255*(.35+.65*(.5-.5*math.cos(now*6)))) if kind == 'loading' else 255
  tint = rl.Color(255, 255, 255, alpha)
  color = rl.Color(100, 220, 150, 255) if kind == 'active' else rl.Color(255, 190, 70, 255) if kind == 'failed' else rl.WHITE
  font = gui_app.font(FontWeight.MEDIUM)
  x, y, w, h = geometry(rect, variant)
  rl.draw_rectangle_rec(rl.Rectangle(x, y, w, h), rl.Color(0, 0, 0, 155))
  rl.draw_texture_ex(texture, rl.Vector2(x+4, y+(h-icon_h)/2), 0., 1., tint)
  fields = status_fields(draw_status.link)
  connection = fields['connection']
  # Connection and model execution remain separate even while a replacement downloads.
  if compact:
    stage = fields['loading'].split(' ')[0]
    loading_short = {'download':'DL', 'upload':'UP', 'build':'BUILD', 'compile':'BUILD',
                     'connect':'LINK', 'waiting':'WAIT', 'failed':'FAIL', 'ready':'READY'}.get(stage, stage.upper()[:5])
    loading_short += fields['percent']
    lines = [('JL ' + {'CONNECTED':'ON', 'OFFLINE':'OFF', 'UNKNOWN':'?'}.get(connection, connection), 10),
             ('eGPU ' + short, 10), (loading_short, 9)]
    tx, ty, available = x+52, y+2, w-56
  else:
    lines = [('Jetlink · ' + connection + ' · ' + fields['transport'], 20),
             ('eGPU · ' + label + ' · ' + fields['loading'], 20)]
    tx, ty, available = x+86, y+4, w-94
  for value, size in lines:
    measured = measure_text_cached(font, value, size).x
    if measured > available:
      size *= available/measured
    rl.draw_text_ex(font, value, rl.Vector2(tx, ty), size, 0, color)
    ty += 11 if compact else 25
