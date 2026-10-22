"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
import os
import time

import pyray as rl

from openpilot.common.hardware import HARDWARE
from openpilot.common.params import Params
from openpilot.system.ui.lib.application import gui_app, FontWeight
from openpilot.system.ui.lib.text_measure import measure_text_cached
from openpilot.system.ui.widgets import Widget


class ScreenSaverSP(Widget):
  def __init__(self, params: Params | None = None):
    super().__init__()
    self.set_rect(rl.Rectangle(0, 0, gui_app.width, gui_app.height))
    self._params = params or Params()
    self._is_mici = HARDWARE.get_device_type() == 'mici' or (HARDWARE.get_device_type() == "pc" and os.getenv("BIG") != "1")

    self.x = 0.0
    self.y = 100.0
    self.vx = 120.0 if self._is_mici else 300.0
    self.vy = 70.0 if self._is_mici else 200.0
    self._hue = 150
    self.color = rl.color_from_hsv(self._hue, 1, 1)

    self.text = "sunnypilot"
    # 屏保图片（存在则优先显示图片，否则显示漂动的文字）
    self._photo_path = os.getenv("SCREENSAVER_PHOTO", "/data/screensaver_photo.png")
    self._tex = None
    self._tex_tried = False
    self.font_size = 50 if self._is_mici else 200
    self._start_time = None
    self._dismiss = False
    self._screensaver_timeout = 300
    self._hit_last_frame = False

  @property
  def is_active(self) -> bool:
    return self._start_time is not None and not self._dismiss

  @property
  def was_dismissed(self) -> bool:
    return self._dismiss

  def initialize(self):
    self._screensaver_timeout = self._params.get("ScreenSaverTimeout", return_default=True)
    if self._start_time is None:
      self._start_time = time.monotonic()
    self._dismiss = False

  def hide_event(self):
    super().hide_event()
    self._dismiss = False
    self._start_time = None

  def _handle_mouse_release(self, mouse_pos):
    self._dismiss = True
    self._start_time = None
    gui_app.pop_widget()
    return super()._handle_mouse_release(mouse_pos)

  def _update_state(self):
    super()._update_state()

    # 优先尝试加载屏保图片
    if not self._tex_tried:
      self._tex_tried = True
      try:
        if os.path.exists(self._photo_path):
          # 高度与文字一致（约 230px），宽度等比缩放 ✓
          _target_h = 230 if not self._is_mici else 60
          self._tex = gui_app.texture(self._photo_path, gui_app.width, _target_h, keep_aspect_ratio=True)
      except Exception:
        self._tex = None

    if self._tex is not None and self._tex.width > 0:
      self.logo_width = float(self._tex.width)
      self.logo_height = float(self._tex.height)
    else:
      self.font = gui_app.font(FontWeight.AUDIOWIDE)
      text_size = measure_text_cached(self.font, self.text, self.font_size, 0)
      self.logo_width = text_size.x
      self.logo_height = text_size.y

    if self._start_time and time.monotonic() - self._start_time > self._screensaver_timeout:
      self._dismiss = True
      self._start_time = None

    dt = rl.get_frame_time()

    self.x += self.vx * dt
    self.y += self.vy * dt

    hit_x = hit_y = False
    if self.x + self.logo_width > self.rect.width:
      self.vx *= -1
      self.x = self.rect.width - self.logo_width
      hit_x = True
    elif self.x < 0:
      self.vx *= -1
      self.x = 0
      hit_x = True

    if self.y + self.logo_height > self.rect.height:
      self.vy *= -1
      self.y = self.rect.height - self.logo_height
      hit_y = True
    elif self.y < 0:
      self.vy *= -1
      self.y = 0
      hit_y = True

    hit = hit_x or hit_y
    if hit and not self._hit_last_frame:
      while self._hue_dist((new_hue := rl.get_random_value(0, 360)), self._hue) < 120:
        pass
      self._hue = new_hue
      self.color = rl.color_from_hsv(self._hue, 1, 1)
    self._hit_last_frame = hit

  @staticmethod
  def _hue_dist(a, b):
    d = abs(a - b)
    return min(d, 360 - d)

  def _render(self, rect: rl.Rectangle):
    self.set_rect(rect)
    rl.clear_background(rl.BLACK)
    if self._tex is not None and self._tex.width > 0:
      # 画图片（保留弹跳移动 ✓ 不染色 ✓）
      rl.draw_texture(self._tex, int(self.x), int(self.y), rl.WHITE)
    else:
      rl.draw_text_ex(self.font, self.text, rl.Vector2(int(self.x), int(self.y)), self.font_size, 0, self.color)
    return -1
