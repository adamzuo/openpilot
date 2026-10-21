import time
import pyray as rl
from collections.abc import Callable
from enum import IntEnum
from openpilot.common.params import Params
from openpilot.selfdrive.ui.widgets.offroad_alerts import UpdateAlert, OffroadAlert
from openpilot.selfdrive.ui.widgets.exp_mode_button import ExperimentalModeButton
from openpilot.selfdrive.ui.widgets.prime import PrimeWidget
from openpilot.selfdrive.ui.widgets.setup import SetupWidget
from openpilot.system.ui.lib.text_measure import measure_text_cached
from openpilot.system.ui.lib.application import gui_app, FontWeight, MousePos, TextAlignment
from openpilot.system.ui.lib.multilang import tr, trn
from openpilot.system.ui.widgets.label import gui_label
from openpilot.system.ui.widgets import Widget

HEADER_HEIGHT = 80
HEAD_BUTTON_FONT_SIZE = 40
CONTENT_MARGIN = 40
SPACING = 25
RIGHT_COLUMN_WIDTH = 750
REFRESH_INTERVAL = 10.0


class HomeLayoutState(IntEnum):
  HOME = 0
  UPDATE = 1
  ALERTS = 2


class HomeLayout(Widget):
  def __init__(self):
    super().__init__()
    self.params = Params()

    self.update_alert = UpdateAlert()
    self.offroad_alert = OffroadAlert()

    self._layout_widgets = {HomeLayoutState.UPDATE: self.update_alert, HomeLayoutState.ALERTS: self.offroad_alert}

    self.current_state = HomeLayoutState.HOME
    self.last_refresh = 0
    self.settings_callback: Callable[[], None] | None = None

    self.update_available = False
    self.alert_count = 0
    self._version_text = ""
    self._home_logo = gui_app.texture("../../sunnypilot/selfdrive/assets/logo.png", 220, 220)
    self._home_banner = gui_app.texture("../../sunnypilot/selfdrive/assets/home_banner.png", 800, 292)
    self._prev_update_available = False
    self._prev_alerts_present = False

    self.header_rect = rl.Rectangle(0, 0, 0, 0)
    self.content_rect = rl.Rectangle(0, 0, 0, 0)
    self.left_column_rect = rl.Rectangle(0, 0, 0, 0)
    self.right_column_rect = rl.Rectangle(0, 0, 0, 0)

    self.update_notif_rect = rl.Rectangle(0, 0, 200, HEADER_HEIGHT - 10)
    self.alert_notif_rect = rl.Rectangle(0, 0, 220, HEADER_HEIGHT - 10)

    self._prime_widget = PrimeWidget()
    self._setup_widget = SetupWidget()

    self._exp_mode_button = ExperimentalModeButton()
    self._setup_callbacks()

  def show_event(self):
    super().show_event()
    self._exp_mode_button.show_event()
    self.last_refresh = time.monotonic()
    self._refresh()

  def _setup_callbacks(self):
    self.update_alert.set_dismiss_callback(lambda: self._set_state(HomeLayoutState.HOME))
    self.offroad_alert.set_dismiss_callback(lambda: self._set_state(HomeLayoutState.HOME))
    self._exp_mode_button.set_click_callback(lambda: self.settings_callback() if self.settings_callback else None)

  def set_settings_callback(self, callback: Callable):
    self.settings_callback = callback

  def _set_state(self, state: HomeLayoutState):
    # propagate show/hide events
    if state != self.current_state:
      if state == HomeLayoutState.HOME:
        self._exp_mode_button.show_event()

      if state in self._layout_widgets:
        self._layout_widgets[state].show_event()
      if self.current_state in self._layout_widgets:
        self._layout_widgets[self.current_state].hide_event()

    self.current_state = state

  def _render(self, rect: rl.Rectangle):
    current_time = time.monotonic()
    if current_time - self.last_refresh >= REFRESH_INTERVAL:
      self._refresh()
      self.last_refresh = current_time

    self._render_header()

    # Render content based on current state
    if self.current_state == HomeLayoutState.HOME:
      self._render_home_content()
    elif self.current_state == HomeLayoutState.UPDATE:
      self._render_update_view()
    elif self.current_state == HomeLayoutState.ALERTS:
      self._render_alerts_view()

  def _update_state(self):
    self.header_rect = rl.Rectangle(
      self._rect.x + CONTENT_MARGIN, self._rect.y + CONTENT_MARGIN, self._rect.width - 2 * CONTENT_MARGIN, HEADER_HEIGHT
    )

    content_y = self._rect.y + CONTENT_MARGIN + HEADER_HEIGHT + SPACING
    content_height = self._rect.height - CONTENT_MARGIN - HEADER_HEIGHT - SPACING - CONTENT_MARGIN

    self.content_rect = rl.Rectangle(
      self._rect.x + CONTENT_MARGIN, content_y, self._rect.width - 2 * CONTENT_MARGIN, content_height
    )

    left_width = self.content_rect.width - RIGHT_COLUMN_WIDTH - SPACING

    self.left_column_rect = rl.Rectangle(self.content_rect.x, self.content_rect.y, left_width, self.content_rect.height)

    self.right_column_rect = rl.Rectangle(
      self.content_rect.x + left_width + SPACING, self.content_rect.y, RIGHT_COLUMN_WIDTH, self.content_rect.height
    )

    self.update_notif_rect.x = self.header_rect.x
    self.update_notif_rect.y = self.header_rect.y + (self.header_rect.height - 60) // 2

    notif_x = self.header_rect.x + (220 if self.update_available else 0)
    self.alert_notif_rect.x = notif_x
    self.alert_notif_rect.y = self.header_rect.y + (self.header_rect.height - 60) // 2

  def _handle_mouse_release(self, mouse_pos: MousePos):
    super()._handle_mouse_release(mouse_pos)

    if self.update_available and rl.check_collision_point_rec(mouse_pos, self.update_notif_rect):
      self._set_state(HomeLayoutState.UPDATE)
    elif self.alert_count > 0 and rl.check_collision_point_rec(mouse_pos, self.alert_notif_rect):
      self._set_state(HomeLayoutState.ALERTS)

  def _render_header(self):
    font = gui_app.font(FontWeight.MEDIUM)

    version_text_width = self.header_rect.width

    # Update notification button
    if self.update_available:
      version_text_width -= self.update_notif_rect.width

      # Highlight if currently viewing updates
      highlight_color = rl.Color(75, 95, 255, 255) if self.current_state == HomeLayoutState.UPDATE else rl.Color(54, 77, 239, 255)
      rl.draw_rectangle_rounded(self.update_notif_rect, 0.3, 10, highlight_color)

      text = tr("UPDATE")
      text_size = measure_text_cached(font, text, HEAD_BUTTON_FONT_SIZE)
      text_x = self.update_notif_rect.x + (self.update_notif_rect.width - text_size.x) // 2
      text_y = self.update_notif_rect.y + (self.update_notif_rect.height - text_size.y) // 2
      rl.draw_text_ex(font, text, rl.Vector2(int(text_x), int(text_y)), HEAD_BUTTON_FONT_SIZE, 0, rl.WHITE)

    # Alert notification button
    if self.alert_count > 0:
      version_text_width -= self.alert_notif_rect.width

      # Highlight if currently viewing alerts
      highlight_color = rl.Color(255, 70, 70, 255) if self.current_state == HomeLayoutState.ALERTS else rl.Color(226, 44, 44, 255)
      rl.draw_rectangle_rounded(self.alert_notif_rect, 0.3, 10, highlight_color)

      alert_text = trn("{} ALERT", "{} ALERTS", self.alert_count).format(self.alert_count)
      text_size = measure_text_cached(font, alert_text, HEAD_BUTTON_FONT_SIZE)
      text_x = self.alert_notif_rect.x + (self.alert_notif_rect.width - text_size.x) // 2
      text_y = self.alert_notif_rect.y + (self.alert_notif_rect.height - text_size.y) // 2
      rl.draw_text_ex(font, alert_text, rl.Vector2(int(text_x), int(text_y)), HEAD_BUTTON_FONT_SIZE, 0, rl.WHITE)

    # Version text (right aligned)
    if self.update_available or self.alert_count > 0:
      version_text_width -= SPACING * 1.5

    version_rect = rl.Rectangle(self.header_rect.x + self.header_rect.width - version_text_width, self.header_rect.y,
                                version_text_width, self.header_rect.height)
    gui_label(version_rect, self._version_text, 48, rl.WHITE, alignment=TextAlignment.RIGHT)

  # 自定义照片缓存（避免每帧重新加载）
  _home_photo_cache = {"key": None, "tex": None}

  def _render_home_content(self):
    """offroad 首页：可选自定义照片 + 标题 + 副标题

    放照片：把图片传到设备 /data/home_photo.png（或 .jpg），即可自动显示
    换照片：覆盖该文件后重启（或改改动就会重新加载）
    不要照片：删掉该文件即可
    改文案：改下面 title / tagline
    """
    import os as _os

    font = gui_app.high_res_text_font(FontWeight.BOLD)
    tag_font = gui_app.high_res_text_font(FontWeight.NORMAL)

    title = "openpilot"
    tagline = "Ready to drive"
    title_size = 100
    tag_size = 42
    gap = 30

    cx = self.content_rect.x + self.content_rect.width / 2
    cy = self.content_rect.y + self.content_rect.height / 2

    # ---------- 自定义照片 ----------
    photo_tex = None
    photo_draw_w = photo_draw_h = 0
    for path in ("/data/home_photo.png", "/data/home_photo.jpg", "/data/home_photo.jpeg"):
      if not _os.path.exists(path):
        continue
      try:
        key = (path, _os.path.getmtime(path))
        if self._home_photo_cache["key"] != key:
          old = self._home_photo_cache["tex"]
          if old is not None:
            try: rl.unload_texture(old)
            except Exception: pass
          self._home_photo_cache["tex"] = rl.load_texture(path)
          self._home_photo_cache["key"] = key
        photo_tex = self._home_photo_cache["tex"]
      except Exception:
        photo_tex = None
      break

    if photo_tex is not None:
      # 照片最多占内容区的 45% 高、60% 宽，等比缩放
      max_w = self.content_rect.width * 0.60
      max_h = self.content_rect.height * 0.45
      scale = min(max_w / photo_tex.width, max_h / photo_tex.height, 1.0)
      photo_draw_w = photo_tex.width * scale
      photo_draw_h = photo_tex.height * scale

    # ---------- 文案尺寸 ----------
    ts = measure_text_cached(font, title, title_size) if title else None
    gs = measure_text_cached(tag_font, tagline, tag_size) if tagline else None

    photo_block = (photo_draw_h + 46) if photo_tex is not None else 0
    text_block = (ts.y if ts else 0) + ((gap + gs.y) if gs else 0)
    total = photo_block + text_block
    y = int(cy - total / 2)

    # ---------- 画照片 ----------
    if photo_tex is not None:
      rl.draw_texture_ex(photo_tex,
                         rl.Vector2(int(cx - photo_draw_w / 2), y),
                         0, scale, rl.WHITE)
      y = int(y + photo_block)

    # ---------- 画标题 ----------
    if ts:
      rl.draw_text_ex(font, title, rl.Vector2(int(cx - ts.x / 2), y), title_size, 0, rl.WHITE)
      y = int(y + ts.y + gap)

    # ---------- 画副标题 ----------
    if gs:
      rl.draw_text_ex(tag_font, tagline, rl.Vector2(int(cx - gs.x / 2), y),
                      tag_size, 0, rl.Color(150, 150, 150, 255))

  def _render_update_view(self):
    self.update_alert.render(self.content_rect)

  def _render_alerts_view(self):
    self.offroad_alert.render(self.content_rect)

  def _render_left_column(self):
    self._prime_widget.render(self.left_column_rect)

  def _render_right_column(self):
    exp_height = 125
    exp_rect = rl.Rectangle(
      self.right_column_rect.x, self.right_column_rect.y, self.right_column_rect.width, exp_height
    )
    self._exp_mode_button.render(exp_rect)

    setup_rect = rl.Rectangle(
      self.right_column_rect.x,
      self.right_column_rect.y + exp_height + SPACING,
      self.right_column_rect.width,
      self.right_column_rect.height - exp_height - SPACING,
    )
    self._setup_widget.render(setup_rect)

  def _refresh(self):
    self._version_text = self._get_version_text()
    update_available = self.update_alert.refresh()
    alert_count = self.offroad_alert.refresh()
    alerts_present = alert_count > 0

    # Show panels on transition from no alert/update to any alerts/update
    if not update_available and not alerts_present:
      self._set_state(HomeLayoutState.HOME)
    elif update_available and ((not self._prev_update_available) or (not alerts_present and self.current_state == HomeLayoutState.ALERTS)):
      self._set_state(HomeLayoutState.UPDATE)
    elif alerts_present and ((not self._prev_alerts_present) or (not update_available and self.current_state == HomeLayoutState.UPDATE)):
      self._set_state(HomeLayoutState.ALERTS)

    self.update_available = update_available
    self.alert_count = alert_count
    self._prev_update_available = update_available
    self._prev_alerts_present = alerts_present

  def _get_version_text(self) -> str:
    # Brand name removed from the home header; keep only the version description.
    return self.params.get("UpdaterCurrentDescription") or ""
