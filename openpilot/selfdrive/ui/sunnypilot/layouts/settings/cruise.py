"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
import threading
from enum import IntEnum

from openpilot.selfdrive.ui.sunnypilot.layouts.settings.cruise_sub_layouts.speed_limit_settings import SpeedLimitSettingsLayout
from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.system.ui.lib.multilang import tr, tr_noop
from openpilot.system.ui.lib.text_measure import measure_text_cached
import pyray as rl

from openpilot.system.ui.sunnypilot.widgets.list_view import ListItemSP, toggle_item_sp, option_item_sp, simple_button_item_sp
from openpilot.system.ui.sunnypilot.widgets.option_control import (
  BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_SPACING, CONTAINER_PADDING, BUTTON_FONT_SIZE, OptionControlSP,
)
from openpilot.system.ui.widgets import Widget
from openpilot.system.ui.widgets.scroller_tici import Scroller
from openpilot.system.ui.sunnypilot.lib.styles import style



class CFDApplyControlSP(OptionControlSP):
  """C3XL: CFD 行控件 = [−][值][+] 后面直接跟一个真正的「重启」按钮（整行一个控件）"""
  RESTART_W = 150
  RESTART_GAP = 20

  def __init__(self, *args, on_restart=None, **kwargs):
    super().__init__(*args, **kwargs)
    self._on_restart = on_restart
    self._restart_rect = None

  SHOW_RESTART = True

  def _render(self, rect):
    if self._rect.width == 0 or self._rect.height == 0 or not self.is_visible:
      return

    control_width = (BUTTON_WIDTH * 2) + self.label_width + (BUTTON_SPACING * 2)
    total_width = control_width + (CONTAINER_PADDING * 2)
    full_width = total_width + self.RESTART_GAP + self.RESTART_W
    self._rect.width = full_width

    start_x = self._rect.x + self._rect.width - full_width
    component_y = rect.y + (rect.height - BUTTON_HEIGHT) / 2

    # — 档位容器 [ −  值  + ] —
    self.container_rect = rl.Rectangle(start_x, component_y, total_width, BUTTON_HEIGHT)
    rl.draw_rectangle_rounded(self.container_rect, 0.2, 20, style.OPTION_CONTROL_CONTAINER_BG)

    self.minus_btn_rect = rl.Rectangle(start_x, component_y, BUTTON_WIDTH + CONTAINER_PADDING, BUTTON_HEIGHT)
    label_x = start_x + CONTAINER_PADDING + BUTTON_WIDTH + BUTTON_SPACING
    self.label_rect = rl.Rectangle(label_x, component_y, self.label_width, BUTTON_HEIGHT)
    plus_x = label_x + self.label_width + BUTTON_SPACING
    self.plus_btn_rect = rl.Rectangle(plus_x, component_y, BUTTON_WIDTH + CONTAINER_PADDING, BUTTON_HEIGHT)

    self._minus_enabled = self.enabled and self.current_value > self.min_value
    self._plus_enabled = self.enabled and self.current_value < self.max_value
    self._render_button(self.minus_btn_rect, "-", self._minus_enabled)
    self._render_value_label()
    self._render_button(self.plus_btn_rect, "+", self._plus_enabled)

    # — 重启按钮（最右，带背景/边框，像真按钮）—
    self._restart_rect = rl.Rectangle(start_x + total_width + self.RESTART_GAP, component_y,
                                      self.RESTART_W, BUTTON_HEIGHT)
    if self.SHOW_RESTART:
      self._render_restart_button()

  def _render_restart_button(self):
    r = self._restart_rect
    if r is None:
      return
    mouse_pos = rl.get_mouse_position()
    pressed = (self.enabled and rl.check_collision_point_rec(mouse_pos, r) and self._touch_valid()
               and rl.is_mouse_button_down(rl.MouseButton.MOUSE_BUTTON_LEFT))
    bg = style.OPTION_CONTROL_BTN_PRESSED if pressed else style.OPTION_CONTROL_CONTAINER_BG
    rl.draw_rectangle_rounded(r, 0.2, 20, bg)
    rl.draw_rectangle_rounded_lines_ex(r, 0.2, 20, 3, style.ITEM_TEXT_COLOR if self.enabled else style.ITEM_DISABLED_TEXT_COLOR)
    label = tr("重启")
    text_color = style.ITEM_TEXT_COLOR if self.enabled else style.ITEM_DISABLED_TEXT_COLOR
    size = measure_text_cached(self._font, label, BUTTON_FONT_SIZE)
    rl.draw_text_ex(self._font, label,
                    rl.Vector2(r.x + (r.width - size.x) / 2, r.y + (r.height - size.y) / 2),
                    BUTTON_FONT_SIZE, 0, text_color)

  def _handle_mouse_release(self, mouse_pos):
    if self._restart_rect is not None and rl.check_collision_point_rec(mouse_pos, self._restart_rect):
      if self.enabled and callable(self._on_restart):
        self._on_restart()
      return
    super()._handle_mouse_release(mouse_pos)

class CFDSpaceControlSP(CFDApplyControlSP):
  """与 CFD 行同宽但不画重启按钮 → 让上下两行的 [−][值][+] 对齐"""
  SHOW_RESTART = False


class PanelType(IntEnum):
  CRUISE = 0
  SLA = 1


ICBM_DESC = tr_noop("When enabled, sunnypilot will attempt to manage the built-in cruise control buttons " +
                    "by emulating button presses for limited longitudinal control.")
ICMB_UNAVAILABLE = tr_noop("Intelligent Cruise Button Management is currently unavailable on this platform.")
ICMB_UNAVAILABLE_LONG_AVAILABLE = tr_noop("Disable the sunnypilot Longitudinal Control (alpha) toggle to allow Intelligent Cruise Button Management.")
ICMB_UNAVAILABLE_LONG_UNAVAILABLE = tr_noop("sunnypilot Longitudinal Control is the default longitudinal control for this platform.")

ACC_ENABLED_DESCRIPTION = tr_noop("Enable custom Short & Long press increments for cruise speed increase/decrease.")
ACC_NOLONG_DESCRIPTION = tr_noop("This feature can only be used with sunnypilot longitudinal control enabled.")
ACC_PCMCRUISE_DISABLED_DESCRIPTION = tr_noop("This feature is not supported on this platform due to vehicle limitations.")
ONROAD_ONLY_DESCRIPTION = tr_noop("Start the vehicle to check vehicle compatibility.")


class CruiseLayout(Widget):
  def __init__(self):
    super().__init__()
    self._current_panel = PanelType.CRUISE
    self._speed_limit_layout = SpeedLimitSettingsLayout(lambda: self._set_current_panel(PanelType.CRUISE))

    items = self._initialize_items()
    self._scroller = Scroller(items, line_separator=True, spacing=0)

  def _initialize_items(self):

    self.icbm_toggle = toggle_item_sp(
      title=tr("Intelligent Cruise Button Management (ICBM) (Alpha)"),
      description="",
      param="IntelligentCruiseButtonManagement")

    # label_width = 90 + 重启按钮宽(150) + 间隔(20) → [−] 与 CFD 行对齐，[+] 顶到最右侧
    self.c3xl_overrun = option_item_sp(
      title=tr("C3XL Overrun"),
      description=tr("超过定速时如何处理。0=原厂(立刻减速)  1=滑行优先(超5km/h内滑行)  2=极温和。行车中也可调。"),
      param="C3XLOverrun",
      min_value=0, max_value=2, value_change_step=1,
      label_width=260,
      label_callback=lambda v: {0: "原厂", 1: "滑行优先", 2: "极温和"}.get(int(v), str(v)),
      inline=True)

    self.accel_boost_toggle = toggle_item_sp(
      title=tr("Accel Boost"),
      description=tr("Adds a small push on top of sunnypilot's own acceleration. Eases off in curves and when slowing or stopping. Never exceeds your set speed."),
      param="AccelBoostSP")

    self.scc_v_toggle = toggle_item_sp(
      title=tr("Smart Cruise Control - Vision"),
      description=tr("Use vision path predictions to estimate the appropriate speed to drive through turns ahead."),
      param="SmartCruiseControlVision")

    self.scc_m_toggle = toggle_item_sp(
      title=tr("Smart Cruise Control - Map"),
      description=tr("Use map data to estimate the appropriate speed to drive through turns ahead."),
      param="SmartCruiseControlMap")

    self.c3xl_follow_profile = ListItemSP(
      title=tr("CFD"),
      description=tr("CFD = C3XL Follow Distance Profile. 原厂(远)/温和/激进/折中；仅停车可改；点左侧「重启」应用并重启 openpilot。"),
      action_item=CFDApplyControlSP(
        "C3XLFollowProfile", 0, 3, 1,
        lambda: ui_state.is_offroad(),
        label_width=90,
        label_callback=lambda v: {0: "原厂", 1: "温和", 2: "激进", 3: "折中", 4: "A紧", 5: "B更紧", 6: "C折中"}.get(int(v), str(v)),
        on_restart=self._apply_follow_profile,
      ),
    )

    self.custom_acc_toggle = toggle_item_sp(
      title=tr("Custom ACC Speed Increments"),
      description="",
      param="CustomAccIncrementsEnabled",
      callback=self._on_custom_acc_toggle)

    self.custom_acc_short_increment = option_item_sp(
      title=tr("Short Press Increment"),
      param="CustomAccShortPressIncrement",
      min_value=1, max_value=10, value_change_step=1,
      inline=True)

    self.custom_acc_long_increment = option_item_sp(
      title=tr("Long Press Increment"),
      param="CustomAccLongPressIncrement",
      value_map={1: 1, 2: 5, 3: 10},
      min_value=1, max_value=6, value_change_step=1,
      inline=True)

    self.sla_settings_button = simple_button_item_sp(
      button_text=lambda: tr("Speed Limit"),
      button_width=800,
      callback=lambda: self._set_current_panel(PanelType.SLA)
    )

    items = [
    #   self.icbm_toggle,
      self.scc_v_toggle,
      self.scc_m_toggle,
      self.custom_acc_toggle,
      self.custom_acc_short_increment,
      self.custom_acc_long_increment,
      self.sla_settings_button,
      self.c3xl_follow_profile,
      self.accel_boost_toggle,
      self.c3xl_overrun,
    ]
    return items


  def _apply_follow_profile(self) -> None:
    """C3XL: 手动应用跟车档位 → 重启 openpilot（仅停车可用）"""
    if not ui_state.is_offroad():
      return
    self._restart_openpilot()

  def _restart_openpilot(self) -> None:
    import subprocess
    try:
      subprocess.Popen(["sudo", "-n", "systemctl", "restart", "comma"])
    except Exception:
      pass

  def _render(self, rect):
    if self._current_panel == PanelType.SLA:
      self._speed_limit_layout.render(rect)
    else:
      self._scroller.render(rect)

  def show_event(self):
    self._set_current_panel(PanelType.CRUISE)
    self._scroller.show_event()
    self.icbm_toggle.show_description(True)
    self.custom_acc_toggle.show_description(True)

  def _set_current_panel(self, panel: PanelType):
    self._current_panel = panel
    if panel == PanelType.SLA:
      self._speed_limit_layout.show_event()

  def _update_state(self):
    super()._update_state()

    if ui_state.CP is not None and ui_state.CP_SP is not None:
      has_icbm = ui_state.has_icbm
      has_long = ui_state.has_longitudinal_control

      if ui_state.CP_SP.intelligentCruiseButtonManagementAvailable and not has_long:
        self.icbm_toggle.action_item.set_enabled(ui_state.is_offroad())
        self.icbm_toggle.set_description(tr(ICBM_DESC))
      else:
        ui_state.params.remove("IntelligentCruiseButtonManagement")
        self.icbm_toggle.action_item.set_enabled(False)

        long_desc = ICMB_UNAVAILABLE
        if has_long:
          if ui_state.CP.alphaLongitudinalAvailable:
            long_desc += " " + ICMB_UNAVAILABLE_LONG_AVAILABLE
          else:
            long_desc += " " + ICMB_UNAVAILABLE_LONG_UNAVAILABLE

        new_desc = "<b>" + tr(long_desc) + "</b>\n\n" + tr(ICBM_DESC)
        if self.icbm_toggle.description != new_desc:
          self.icbm_toggle.set_description(new_desc)
          self.icbm_toggle.show_description(True)

      if has_long or has_icbm:
        self.custom_acc_toggle.action_item.set_enabled(((has_long and not ui_state.CP.pcmCruise) or has_icbm) and ui_state.is_offroad())
        self.scc_v_toggle.action_item.set_enabled(True)
        self.scc_m_toggle.action_item.set_enabled(True)
      else:
        ui_state.params.remove("CustomAccIncrementsEnabled")
        ui_state.params.remove("SmartCruiseControlVision")
        ui_state.params.remove("SmartCruiseControlMap")
        self.custom_acc_toggle.action_item.set_enabled(False)
        self.scc_v_toggle.action_item.set_enabled(False)
        self.scc_m_toggle.action_item.set_enabled(False)

    else:
      has_icbm = has_long = False
      self.icbm_toggle.action_item.set_enabled(False)
      self.icbm_toggle.set_description(tr(ONROAD_ONLY_DESCRIPTION))

    show_custom_acc_desc = False

    if ui_state.is_offroad():
      new_custom_acc_desc = tr(ONROAD_ONLY_DESCRIPTION)
      show_custom_acc_desc = True
    else:
      if has_long or has_icbm:
        if has_long and ui_state.CP.pcmCruise:
          new_custom_acc_desc = tr(ACC_PCMCRUISE_DISABLED_DESCRIPTION)
          show_custom_acc_desc = True
        else:
          new_custom_acc_desc = tr(ACC_ENABLED_DESCRIPTION)
      else:
        new_custom_acc_desc = tr(ACC_NOLONG_DESCRIPTION)
        show_custom_acc_desc = True
        self.custom_acc_toggle.action_item.set_state(False)

    if self.custom_acc_toggle.description != new_custom_acc_desc:
      self.custom_acc_toggle.set_description(new_custom_acc_desc)
      if show_custom_acc_desc:
        self.custom_acc_toggle.show_description(True)

    self._on_custom_acc_toggle(self.custom_acc_toggle.action_item.get_state())

  def _on_custom_acc_toggle(self, state):
    self.custom_acc_short_increment.set_visible(state)
    self.custom_acc_long_increment.set_visible(state)
    self.custom_acc_short_increment.action_item.set_enabled(self.custom_acc_toggle.action_item.enabled)
    self.custom_acc_long_increment.action_item.set_enabled(self.custom_acc_toggle.action_item.enabled)
