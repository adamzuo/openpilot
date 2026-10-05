"""Jetlink settings/status shared by the Device panel and ADB toggle."""
from dragonpilot import jetlink_adapter

MODE_LABELS = ("關閉 / Off", "USB（Android、Mac、Jetson、Linux）", "iOS（iPhone、iPad）")


def mode_index(params) -> int:
  try:
    value = int(params.get(jetlink_adapter.KEYS.link) or 0)
  except (TypeError, ValueError):
    return 0
  return value if 0 <= value < len(MODE_LABELS) else 0


def adb_blocked(params) -> bool:
  return mode_index(params) != 0


def set_mode(params, index: int) -> None:
  if type(index) is not int or not 0 <= index < len(MODE_LABELS):
    raise ValueError("invalid Jetlink mode")
  if index:
    params.put_bool("AdbEnabled", False, block=True)
  params.put(jetlink_adapter.KEYS.link, index, block=True)


def status_text(params) -> str:
  if mode_index(params) == 0:
    return "已關閉 / Off"
  view = jetlink_adapter.status()
  if view is None:
    return jetlink_adapter.reason() or "Jetlink 套件不可用"
  if view.reason:
    return str(view.reason)
  if not view.enabled:
    return "Jetlink 未啟用（USB GPU 已占用連線）"
  state = params.get("JetlinkModelState")
  if state == 'vision':
    import json
    from jetlink.vision import fresh_snapshot
    try:
      snapshot = json.loads(params.get("JetlinkVision") or '{}')
      hz = snapshot.get('target_hz', '?')
      if fresh_snapshot(snapshot):
        usage = json.loads(params.get("JetlinkVisionUse") or '{}')
        if usage.get('source') == 'phone':
          return f"手機前車視覺優先 {hz} Hz；道路與轉向使用本機模型"
        reason = usage.get('reason', '')
        guard_reasons = ('local_', 'phone_age_', 'guard_')
        if reason.startswith(guard_reasons):
          return f"本機前車視覺補位 {hz} Hz（快速變化／延遲保護）"
        return f"手機資料已收到 {hz} Hz；目前使用本機前車視覺"
      return f"等待手機新資料 {hz} Hz；使用本機前車視覺"
    except (ValueError, TypeError):
      return "視覺輔助連接中，本機模型持續運作"
  if state == 'running':
    return "外部模型運算中 / Running"
  if state == 'ready':
    return "模型已就緒，等待解除控制 / Ready"
  progress = view.progress or {}
  if progress.get('msg'):
    return str(progress['msg'])
  if view.ready:
    return "外部模型已載入 / Loaded"
  if view.present:
    return f"已連接 / Connected: {view.transport}"
  return "等待連接 / Waiting for Jetlink"
