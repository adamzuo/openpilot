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

  # 状态面板缓存（避免每帧读文件）
  _status_cache = {"t": 0.0, "rows": []}

  def _get_status_rows(self):
    """收集车辆/设备状态（1 秒缓存）"""
    import os, time as _t, shutil
    now = _t.monotonic()
    if now - self._status_cache["t"] < 1.0 and self._status_cache["rows"]:
      return self._status_cache["rows"]

    rows = []
    # 车系（CarParams 里读取，失败则跳过）
    try:
      try:
        from openpilot.cereal import car
      except Exception:
        from cereal import car
      cp = car.CarParams.from_bytes(Params().get("CarParams"))
      rows.append(("车辆", cp.carFingerprint or "--"))
      rows.append(("品牌", str(cp.brand) if cp.brand else "--"))
    except Exception:
      pass

    # 温度（camera / cpu）
    try:
      temps = {}
      for z in os.listdir("/sys/class/thermal"):
        if not z.startswith("thermal_zone"):
          continue
        with open(f"/sys/class/thermal/{z}/type") as f:
          name = f.read().strip()
        with open(f"/sys/class/thermal/{z}/temp") as f:
          temps[name] = int(f.read().strip()) / 1000.0
      if "cpu0-gold-usr" in temps:
        rows.append(("CPU 温度", f"{temps['cpu0-gold-usr']:.0f} °C"))
      if "camera" in temps:
        rows.append(("相机温度", f"{temps['camera']:.0f} °C"))
      if "battery" in temps:
        rows.append(("电池温度", f"{temps['battery']:.0f} °C"))
    except Exception:
      pass

    # 内存
    try:
      with open("/proc/meminfo") as f:
        mem = {l.split(":")[0]: int(l.split()[1]) for l in f if ":" in l}
      total, avail = mem.get("MemTotal", 1), mem.get("MemAvailable", 0)
      rows.append(("内存占用", f"{100 - int(avail * 100 / total)} %"))
    except Exception:
      pass

    # 存储
    try:
      du = shutil.disk_usage("/data")
      rows.append(("存储空间", f"{du.used / 2**30:.1f} / {du.total / 2**30:.0f} GB"))
    except Exception:
      pass

    # 运行时长
    try:
      with open("/proc/uptime") as f:
        up = int(float(f.read().split()[0]))
      rows.append(("运行时长", f"{up // 3600} 小时 {(up % 3600) // 60} 分"))
    except Exception:
      pass

    # IP
    try:
      import socket
      s_ = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
      s_.connect(("8.8.8.8", 80))
      rows.append(("本机 IP", s_.getsockname()[0]))
      s_.close()
    except Exception:
      pass

    # 软件版本
    try:
      rows.append(("软件版本", self._get_version_text() or "--"))
    except Exception:
      pass

    self._status_cache["t"] = now
    self._status_cache["rows"] = rows
    return rows

  # 首页配置缓存（网页端修改 /data/home_config.json 后自动生效）
  _home_cfg_cache = {"key": None, "cfg": {}}

  def _get_home_config(self):
    import os, json
    path = "/data/home_config.json"
    try:
      key = os.path.getmtime(path) if os.path.exists(path) else None
      if key != self._home_cfg_cache["key"]:
        cfg = {}
        if key is not None:
          with open(path, encoding="utf-8") as f:
            cfg = json.load(f)
        self._home_cfg_cache["key"] = key
        self._home_cfg_cache["cfg"] = cfg if isinstance(cfg, dict) else {}
      return self._home_cfg_cache["cfg"]
    except Exception:
      return {}

  def _get_dynamic_rows(self):
    """动态状态（温度/内存/存储/时长/IP/版本），1 秒缓存"""
    import os, time as _t, shutil
    now = _t.monotonic()
    if now - self._status_cache["t"] < 1.0 and self._status_cache["rows"]:
      return self._status_cache["rows"]

    rows = []
    try:
      temps = {}
      for z in os.listdir("/sys/class/thermal"):
        if not z.startswith("thermal_zone"):
          continue
        with open(f"/sys/class/thermal/{z}/type") as f:
          name = f.read().strip()
        with open(f"/sys/class/thermal/{z}/temp") as f:
          temps[name] = int(f.read().strip()) / 1000.0
      for k, lbl in (("cpu0-gold-usr", "CPU 温度"), ("camera", "相机温度"), ("battery", "电池温度")):
        if k in temps:
          rows.append((lbl, f"{temps[k]:.0f} °C"))
    except Exception:
      pass
    try:
      with open("/proc/meminfo") as f:
        mem = {l.split(":")[0]: int(l.split()[1]) for l in f if ":" in l}
      rows.append(("内存", f"{100 - int(mem.get('MemAvailable', 0) * 100 / max(mem.get('MemTotal', 1), 1))} %"))
    except Exception:
      pass
    try:
      du = shutil.disk_usage("/data")
      rows.append(("存储空间", f"{du.used / 2**30:.1f} / {du.total / 2**30:.0f} GB"))
    except Exception:
      pass
    try:
      with open("/proc/uptime") as f:
        up = int(float(f.read().split()[0]))
      rows.append(("运行时长", f"{up // 3600} 小时 {(up % 3600) // 60} 分"))
    except Exception:
      pass
    try:
      import socket
      s_ = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
      s_.connect(("8.8.8.8", 80))
      rows.append(("本机 IP", s_.getsockname()[0]))
      s_.close()
    except Exception:
      pass
    try:
      rows.append(("软件版本", self._get_version_text() or "--"))
    except Exception:
      pass

    self._status_cache["t"] = now
    self._status_cache["rows"] = rows
    return rows

  # 按需生成包含实际字符的 CJK 字体（避免字形缺失显示成 "?"）
  _cjk_font_cache = {}

  # 自定义图片缓存
  _photo_cache = {"key": None, "tex": None}

  def _get_home_photo(self):
    """读取 /data/home_photo.* 并返回纹理

    使用 gui_app.texture() —— 厂商自己的 logo 就用这个 API（安全 ✓）
    内部会做 keep_aspect_ratio 缩放 + 缓存，不会有显存问题
    注意：换图后需要重启 comma 服务（texture 有内部缓存）
    """
    import os
    for path in ("/data/home_photo.png", "/data/home_photo.jpg", "/data/home_photo.jpeg"):
      if not os.path.exists(path):
        continue
      try:
        key = (path, os.path.getmtime(path))
        if self._home_photo_cache.get("key") != key:
          # 关键：用 gui_app.texture（厂商验证过的安全加载方式）
          tex = gui_app.texture(path, 1920, 1080, keep_aspect_ratio=True)
          self._home_photo_cache["tex"] = tex
          self._home_photo_cache["key"] = key
        return self._home_photo_cache["tex"]
      except Exception as e:
        try:
          from openpilot.common.swaglog import cloudlog
          cloudlog.error(f"[home] photo load failed: {e}")
        except Exception:
          print(f"[home] photo load failed: {e}", flush=True)
        return None
    return None

  def _get_cjk_font(self, texts, base_size=48):
    import hashlib
    from pathlib import Path
    chars = set(chr(c) for c in range(32, 127))
    for t in texts:
      if t:
        chars.update(str(t))
    key = hashlib.md5("".join(sorted(chars)).encode("utf-8")).hexdigest()[:10]
    if key in self._cjk_font_cache:
      return self._cjk_font_cache[key]
    try:
      path = Path("/data/openpilot/openpilot/selfdrive/assets/fonts/SourceHanSansSC-Bold.otf")
      if not path.exists():
        return None
      cps = sorted({ord(c) for c in chars if ord(c) > 0})
      buf = rl.ffi.new("int[]", cps)
      font = rl.load_font_ex(str(path), base_size, rl.ffi.cast("int *", buf), len(cps))
      rl.gen_texture_mipmaps(font.texture)
      rl.set_texture_filter(font.texture, rl.TextureFilter.TEXTURE_FILTER_TRILINEAR)
      self._cjk_font_cache[key] = font
      return font
    except Exception as e:
      print(f"[home] CJK font load failed: {e}", flush=True)
      return None

  def _render_home_content(self):
    """offroad 首页：
       - 有 /data/home_photo.* → 图片铺满（不显示任何文字）
       - 无图片 → 显示文字状态面板
    """
    # 每 2 秒让 raylib 截一张图到 /tmp/screenshot.png
    # （设备上没有 screencap/scrot，也没有 /dev/fb0，只能由 UI 自己截）
    try:
      import time as _t
      _now = _t.monotonic()
      if _now - getattr(self, "_last_shot_ts", 0.0) > 2.0:
        self._last_shot_ts = _now
        rl.take_screenshot("/tmp/screenshot.png")
    except Exception:
      pass

    # ---------- 图片模式：铺满整个首页区域，不显示文字 ----------
    _photo = self._get_home_photo()
    if getattr(self, "_photo_logged_key", None) != str(self._home_photo_cache.get("key")):
      self._photo_logged_key = str(self._home_photo_cache.get("key"))
      try:
        from openpilot.common.swaglog import cloudlog
        cloudlog.error(f"[home] photo branch: tex={'YES' if _photo is not None else 'NONE'}, key={self._home_photo_cache.get('key')}")
      except Exception:
        pass
    if _photo is not None:
      # 用「内容区」（顶栏下方）而不是整个布局区，避免遮住右上角版本号
      area = self.content_rect
      scale = min(area.width / _photo.width, area.height / _photo.height)
      dw, dh = _photo.width * scale, _photo.height * scale
      # 支持水平偏移（网页配置 photo_offset_x：正数=右移，负数=左移）
      _pcfg = self._get_home_config()
      try:
        _ox = float(_pcfg.get("photo_offset_x", 0))
      except Exception:
        _ox = 0.0
      _oy = 0.0
      try:
        _oy = float(_pcfg.get("photo_offset_y", 0))
      except Exception:
        _oy = 0.0
      ax = area.x + (area.width - dw) / 2 + _ox
      ay = area.y + (area.height - dh) / 2 + _oy
      rl.draw_texture_ex(_photo, rl.Vector2(int(ax), int(ay)), 0, scale, rl.WHITE)
      return

    """offroad 首页：配置来自 /data/home_config.json（网页端可改）

    配置格式：
      {"title": "车辆状态", "show_dynamic": true,
       "rows": [["车辆", "TOYOTA RAV4"], ["品牌", "toyota"]]}
    """
    cfg = self._get_home_config()
    # 注意：high_res 字体是 Inter，没有中文字形（会显示成 "?"）
    # 用按需字体：把实际要显示的字符都加载进去，任何中文都能显示
    _all_texts = [str(cfg.get("title", "车辆状态"))]
    for r in cfg.get("rows", []):
      if isinstance(r, (list, tuple)) and len(r) >= 2:
        _all_texts += [str(r[0]), str(r[1])]
    _all_texts += ["CPU 温度", "相机温度", "电池温度", "内存占用", "存储空间", "运行时长", "本机 IP", "软件版本"]
    _cjk = self._get_cjk_font(_all_texts)
    if _cjk is not None:
      if getattr(self, "_font_logged", False) is False:
        print("[home] using dynamic CJK font ✓", flush=True)
        self._font_logged = True
      font = lab_font = _cjk
    else:
      font = gui_app.font(FontWeight.BOLD)
      lab_font = gui_app.font(FontWeight.NORMAL)

    title = str(cfg.get("title", "车辆状态"))
    show_dynamic = bool(cfg.get("show_dynamic", True))

    rows = []
    for r in cfg.get("rows", []):
      try:
        if isinstance(r, (list, tuple)) and len(r) >= 2:
          rows.append((str(r[0]), str(r[1])))
        elif isinstance(r, str):
          rows.append((r, ""))
      except Exception:
        pass
    if show_dynamic:
      rows.extend(self._get_dynamic_rows())

    title_size = int(cfg.get("title_size", 64))
    label_size = int(cfg.get("label_size", 40))
    value_size = int(cfg.get("value_size", 40))
    row_h = int(cfg.get("row_h", 70))
    label_w = int(cfg.get("label_w", 240))
    offset_x = int(cfg.get("offset_x", -400))

    cx = self.content_rect.x + self.content_rect.width / 2
    cy = self.content_rect.y + self.content_rect.height / 2

    _ph = 0.0
    _pt = self._get_home_photo()
    if _pt is not None:
      _ph = min(self.content_rect.height * 0.32, _pt.height) + 30
    total_h = _ph + title_size + 46 + row_h * max(len(rows), 1)
    y = int(cy - total_h / 2)

    left_x = int(cx + offset_x)
    value_x = left_x + label_w

    # 自定义图片（/data/home_photo.png），放在标题上方
    photo_tex = self._get_home_photo()
    if photo_tex is not None:
      max_w = self.content_rect.width * 0.50
      max_h = self.content_rect.height * 0.32
      scale = min(max_w / photo_tex.width, max_h / photo_tex.height, 1.0)
      dw, dh = photo_tex.width * scale, photo_tex.height * scale
      rl.draw_texture_ex(photo_tex, rl.Vector2(int(cx - dw / 2), y), 0, scale, rl.WHITE)
      y = int(y + dh + 30)

    # 标题与自定义行左对齐（原来居中，会看起来错位）
    rl.draw_text_ex(font, title, rl.Vector2(left_x, y), title_size, 0, rl.WHITE)
    y = int(y + title_size + 46)
    for label, value in rows:
      rl.draw_text_ex(lab_font, label, rl.Vector2(left_x, y), label_size, 0, rl.Color(150, 150, 150, 255))
      if value:
        # 数值宽度超过「标签列宽」时自动缩小字号（如 IP 地址过长）
        vs = value_size
        try:
          tw = measure_text_cached(lab_font, value, vs)
          tw_x = tw.x if hasattr(tw, "x") else float(tw)
          if tw_x > label_w and tw_x > 0:
            vs = max(18, int(vs * label_w / tw_x))
        except Exception:
          pass
        rl.draw_text_ex(lab_font, value, rl.Vector2(value_x, y), vs, 0, rl.WHITE)
      y += row_h

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
