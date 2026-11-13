"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""

from enum import IntFlag


class ToyotaFlagsSP(IntFlag):
  SMART_DSU = 1
  RADAR_CAN_FILTER = 2
  ZSS = 4
  STOCK_LONGITUDINAL = 8
  STOP_AND_GO_HACK = 16
  TSS2_SMOOTH = 32


# kiBP, kiV for the PCM accel PID with TSS2_SMOOTH
# C3XL: <=80 km/h 保持积极(0.25)，>80 km/h 平缓降到 0.219
# C3XL 20261111: 改回 sunnypilot 官方的 TSS2 smooth ki（原为 0.25/0.219 的调校）
# C3XL 20261113: 折中值（官方 0.46/0.19 与 我们的 0.25/0.219 之间，整体偏大）
TSS2_SMOOTH_KI = ([0., 5., 10., 20., 28., 36.], [0.36, 0.32, 0.28, 0.27, 0.26, 0.23])


class ToyotaSafetyFlagsSP:
  DEFAULT = 0
  UNSUPPORTED_DSU = 1
  GAS_INTERCEPTOR = 2
