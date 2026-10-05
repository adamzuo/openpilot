"""
Copyright (c) 2026-, Zeph Leggett.

This file is part of the Jetlink port for DragonPilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.

Jetlink on dptest: the adapter for local modeld, settings and hardware.
The USB host runs the large model; cameras, calibration, parsing, controls
and CAN remain on comma. No Sunnypilot model manager is required.

The manager and resident gadget owner import this module, so top-level
imports stay standard-library only. Guarded hooks preserve local operation
when Jetlink is unavailable.
"""
from __future__ import annotations

import functools
import os
import threading
from collections import namedtuple
from pathlib import Path

# the version of jetlink.openpilot's API this adapter is written to; any other
# is treated as jetlink being absent, with the reason as the offroad alert
API = 1

# the gadget owner, as manager names the process and selfdrived lists it
OWNER = 'jetlinkd'

# the Jetlink setting, stored as an index: jetlink.openpilot.MODES,
# written out so the panels build the setting without a jetlink checkout
MODES = ('off', 'usb', 'ios')

# dptest declares these keys in common/params_keys.h. No catalog or model
# manager: None selects Jetlink's own default large model.
_Keys = namedtuple('_Keys', 'link offroad progress spec pointers big_model catalog')
KEYS = _Keys(link='JetlinkLink', offroad='IsOffroad', progress='AcceleratorProgress', spec='JetlinkSpec',
             pointers='JetlinkModelPointers', big_model=None, catalog=None)

# comma's chestnut, running and in its ROM (common.hardware.usb): the comma's
# USB-C port hosts one and is never held as a jetlink device beside it. The
# hardware package is too heavy for the owner, so they are written out here
CHESTNUT_IDS = frozenset({(0xADD1, 0x0001), (0x3801, 0x0001), (0x174C, 0x2464), (0x174C, 0x2463)})

# where the build puts the warp for each camera (SConscript) and modeld loads
# it from: in the fork's tree, never in the jetlink submodule (a file there
# leaves it dirty for the updater), and under the *.pkl ignore, which the
# release scripts add past. Not Paths.comma_home(), which on AGNOS is a tmpfs
# overlay: the pickle was gone every boot
WARP_DIR = Path(__file__).resolve().parent / 'models'

OWNER_LOG = Path('/data/log/jetlink-owner.log')

_AGNOS = os.path.isfile('/AGNOS')


def _params_dir() -> Path:
  """The params store's directory, by params.cc and hw.h's rule: PARAMS_ROOT,
  else /data/params on a device and ~/.comma<OPENPILOT_PREFIX>/params
  elsewhere, then /<OPENPILOT_PREFIX, or d>. Per call, so it follows a prefix
  as Params does; the environment only, so it never raises."""
  prefix = os.environ.get('OPENPILOT_PREFIX', '')
  root = os.environ.get('PARAMS_ROOT')
  if root is None:
    root = '/data/params' if _AGNOS else os.path.join(os.environ.get('HOME', ''), '.comma' + prefix, 'params')
  return Path(root) / os.environ.get('OPENPILOT_PREFIX', 'd')


def warp_path(cam_w: int, cam_h: int, model_w: int, model_h: int) -> Path:
  """The warp for one geometry: the build's target and what modeld opens."""
  return WARP_DIR / f'warp_{cam_w}x{cam_h}_{model_w}x{model_h}_tinygrad.pkl'


def owner_config():
  """What jetlinkd needs: data only, so the owner imports nothing heavy."""
  from jetlink.openpilot.interface import Keys, OwnerConfig

  from openpilot.common.basedir import BASEDIR
  # the provisioning run starts in the checkout with the checkout on its path,
  # where launch_chffrplus.sh links jetlink_repo/jetlink in as jetlink
  return OwnerConfig(params_dir=_params_dir(), keys=Keys(**KEYS._asdict()), chestnut_ids=CHESTNUT_IDS,
                     adapter=__name__, cwd=Path(BASEDIR), env={'PYTHONPATH': BASEDIR}, log_file=OWNER_LOG)


def main() -> None:
  """jetlinkd: hold the USB gadget until manager stops this process."""
  from jetlink.openpilot.owner import main as run_owner
  run_owner(owner_config())


def adapter() -> Adapter:
  """The adapter, for jetlink's entry points that run as their own process:
  the provisioning run and the warp build."""
  return Adapter()


class Adapter:
  """jetlink.openpilot.interface.Openpilot over this fork."""

  def publish_vision(self, snapshot):
    """Write auxiliary data off the frame thread; coordinates stay in model space."""
    import json
    from openpilot.common.params import Params
    Params().put("JetlinkVision", json.dumps(snapshot, allow_nan=False), block=True)

  def __init__(self):
    from jetlink.openpilot.interface import Keys

    from openpilot.common.basedir import BASEDIR
    from openpilot.common.swaglog import cloudlog
    self.keys = Keys(**KEYS._asdict())
    self.log = cloudlog
    self.basedir = Path(BASEDIR)
    # one Params per store: constructing one costs 144 us on the comma against
    # 110 us for the read, and the UI reads several five times a second. By
    # store, since a test or a bench runs under its own prefix
    self._stores: dict[Path, object] = {}

  # -- params ---------------------------------------------------------------

  def params_dir(self) -> Path:
    return _params_dir()

  def _params(self):
    where = _params_dir()
    store = self._stores.get(where)
    if store is None:
      from openpilot.common.params import Params
      store = self._stores[where] = Params()
    return store

  def get(self, key: str):
    # read from hardwared and the UI's threads, which UnknownKeyName (a
    # params library older than the key) must not take down
    try:
      return self._params().get(key)
    except Exception:
      return None

  def put(self, key: str, value, *, block: bool = False) -> None:
    self._params().put(key, value, block=block)

  def remove(self, key: str) -> None:
    self._params().remove(key)

  # -- the device -------------------------------------------------------------

  def chestnut_present(self) -> bool:
    # Include boot-ROM identities as well as dptest's running USB GPU.
    # Jetlink must not switch a fitted GPU's controller into device mode.
    for device in Path('/sys/bus/usb/devices').glob('*'):
      try:
        ids = (int((device / 'idVendor').read_text(), 16), int((device / 'idProduct').read_text(), 16))
        if ids in CHESTNUT_IDS:
          return True
      except (OSError, ValueError):
        pass
    return False

  def camera(self) -> tuple[int, int, int, int]:
    # the choice modeld/SConscript makes for a source build
    from openpilot.system.hardware import HARDWARE
    from openpilot.common.transformations.camera import _ar_ox_fisheye, _os_fisheye
    from openpilot.common.transformations.model import MEDMODEL_INPUT_SIZE
    camera = _os_fisheye if HARDWARE.get_device_type() == "mici" else _ar_ox_fisheye
    return camera.width, camera.height, *MEDMODEL_INPUT_SIZE

  def warp_path(self, cam_w: int, cam_h: int, model_w: int, model_h: int) -> Path:
    return warp_path(cam_w, cam_h, model_w, model_h)

  def model_root(self) -> Path:
    return Path('/data/models') if _AGNOS else Path.home() / '.comma' / 'models'

  @property
  def catalog_selector(self) -> int:
    return 0  # This fork has no Sunnypilot model catalog.

  # -- modeld -----------------------------------------------------------------

  def model_face(self):
    """comma's large model's face: stock modeld's Parser, constants and action
    function, and modeld_v2's ModelConstants, which modeld_tinygrad reads off
    the model."""
    from jetlink.openpilot.interface import ModelFace

    from openpilot.selfdrive.modeld.constants import ModelConstants
    from openpilot.selfdrive.modeld.modeld import LAT_SMOOTH_SECONDS, LONG_SMOOTH_SECONDS, get_action_from_model
    from openpilot.selfdrive.modeld.parse_model_outputs import Parser
    from openpilot.system.camerad.cameras.nv12_info import get_nv12_info
    return ModelFace(parser=Parser, frame_size=lambda w, h: get_nv12_info(w, h)[3], desire_len=ModelConstants.DESIRE_LEN,
                     constants=ModelConstants, lat_smooth_seconds=LAT_SMOOTH_SECONDS,
                     long_smooth_seconds=LONG_SMOOTH_SECONDS, get_action_from_model=get_action_from_model)

  def engagement(self):
    """A poller over selfdrived and the car: the large model swaps in only
    while nothing is in control."""
    import cereal.messaging as messaging
    services = ('selfdriveState', 'carState', 'carControl')
    sm = messaging.SubMaster(list(services))

    def engaged(timeout_ms: int) -> bool:
      sm.update(timeout_ms)
      # a service that is missing, late or invalid counts as engaged
      valid = all(sm.seen[s] and sm.alive[s] and sm.valid[s] for s in services)
      cc = sm['carControl']
      # dptest's always-on lateral can be active without enabled.
      return not valid or sm['selfdriveState'].enabled or cc.latActive or cc.longActive
    return engaged

  def event(self, name: str, **fields) -> None:
    self.log.event(name, **fields)

  # -- the build --------------------------------------------------------------

  def make_warp(self, cam_w: int, cam_h: int, model_w: int, model_h: int):
    # Reuse dptest's NV12 sampling, leaving history on the remote host.
    from openpilot.selfdrive.modeld.compile_modeld import NV12Frame, make_frame_prepare
    from openpilot.system.camerad.cameras.nv12_info import get_nv12_info
    from tinygrad import Tensor, Device
    nv12 = NV12Frame(cam_w, cam_h, *get_nv12_info(cam_w, cam_h))
    frame_prepare = make_frame_prepare(nv12, model_w, model_h)

    def warp(tfm, big_tfm, frame, big_frame):
      tfm, big_tfm = tfm.to(Device.DEFAULT), big_tfm.to(Device.DEFAULT)
      frame, big_frame = frame.to(Device.DEFAULT), big_frame.to(Device.DEFAULT)
      Tensor.realize(tfm, big_tfm, frame, big_frame)
      return Tensor.cat(frame_prepare(frame, tfm).unsqueeze(0),
                        frame_prepare(big_frame, big_tfm).unsqueeze(0))

    return warp, nv12.size


# -- what the hooks call ------------------------------------------------------

class _Absent:
  """jetlink's answers when it cannot run here: not checked out (why is
  None), or a package this build cannot use (why says so, as the offroad
  alert, to someone who turned the link on)."""

  def __init__(self, why: str | None):
    self.why = why

  def enabled(self) -> bool:
    return False

  def status(self):
    return None

  def reason(self) -> str | None:
    if self.why is None:
      return None
    # the setting as jetlink reads it, a file: hardwared asks twice a second
    try:
      on = 0 < int((_params_dir() / KEYS.link).read_bytes()) < len(MODES)
    except (OSError, ValueError):
      on = False
    return self.why if on else None

  def prepare(self) -> bool:
    return False

  def attach(self, small, cam_w: int, cam_h: int):
    return None

  def request_shutdown(self, reason: str = '') -> bool:
    return False

  def shutdown_pending(self) -> bool:
    return False

  def should_extend_catalog(self) -> bool:
    return False

  def extend_catalog(self, catalog: dict) -> dict:
    return catalog


_bound = None
_binding = threading.Lock()


def _api():
  """jetlink for this process, bound to the adapter on first use. Kept, the
  null answers included: Python does not cache a failed import, and searching
  the path again on every UI and hardwared call costs more than the call. One
  per process: prepare() and attach() have to reach the same one."""
  global _bound
  if _bound is None:
    with _binding:
      if _bound is None:
        _bound = _bind()
  return _bound


def _bind():
  try:
    import jetlink
    if getattr(jetlink, '__file__', None) is None:
      return _Absent(None)   # an empty jetlink_repo, which Python takes for a namespace package
    import jetlink.openpilot as jl
  except ModuleNotFoundError as e:
    if e.name == 'jetlink':
      return _Absent(None)   # no checkout: the link does not exist on this device
    if (e.name or '').startswith('jetlink.'):
      return _unusable("jetlink package too old for this build", e)
    return _unusable(f"jetlink failed to load: {e}", e)
  except Exception as e:
    return _unusable(f"jetlink failed to load: {type(e).__name__}: {e}", e)
  api = getattr(jl, 'API', None)
  if api != API:
    return _unusable(f"jetlink package API {api}, this build expects {API}")
  try:
    return jl.bind(Adapter())
  except Exception as e:
    return _unusable(f"jetlink failed to start: {type(e).__name__}: {e}", e)


def _unusable(why: str, error: Exception | None = None) -> _Absent:
  _log_failure(why, error)
  return _Absent(why)


# manager, hardwared, the model manager and the UI call in here on every
# device, link on or off, and modeld on every drive: whatever jetlink does
# wrong turns the link off and is logged, and never takes one of them down.
# jetlink's own readers never raise; this is the net under that promise.
# Hook -> the failure last logged for it, cleared by a call that works
_failed_hooks: dict[str, str] = {}


def _log_failure(what: str, error: Exception | None) -> None:
  try:
    from openpilot.common.swaglog import cloudlog
    cloudlog.error("jetlink: %s", what, exc_info=error)
  except Exception:
    pass


def _guarded(default):
  def wrap(hook):
    @functools.wraps(hook)
    def call(*args, **kwargs):
      try:
        result = hook(*args, **kwargs)
      except Exception as e:
        # once per distinct error, as jetlink's readers log: the UI would log
        # a failing status five times a second
        error = f"{type(e).__name__}: {e}"
        if _failed_hooks.get(hook.__name__) != error:
          _failed_hooks[hook.__name__] = error
          _log_failure(f"{hook.__name__}() failed", e)
        return default(*args, **kwargs) if callable(default) else default
      _failed_hooks.pop(hook.__name__, None)
      return result
    return call
  return wrap


@_guarded(False)
def should_run(started: bool, params, CP) -> bool:
  """manager's rule for jetlinkd: the link is on and no chestnut is fitted.
  jetlinkd runs onroad too: a gadget whose owner exits leaves the bus."""
  return _api().enabled()


@_guarded(None)
def status():
  """One snapshot for the UI and the panels (jetlink.openpilot.Status), or
  None when there is no jetlink here."""
  return _api().status()


@_guarded(None)
def reason() -> str | None:
  """Why the link the user turned on cannot run: hardwared's offroad alert.
  Files only, so hardwared can ask twice a second."""
  return _api().reason()


@_guarded(False)
def prepare() -> bool:
  """modeld, before config_realtime_process: will the link join this modeld?
  The GPU's setup has to happen now, or its threads inherit the frame loop's
  realtime priority and core."""
  return _api().prepare()


@_guarded(None)
def attach(small, cam_w: int, cam_h: int):
  """modeld, once the camera is up and `small` is built: the model to run,
  `small` driving until the link has joined; None unless prepare() said yes."""
  return _api().attach(small, cam_w, cam_h)


@_guarded(False)
def request_shutdown(reason: str = '') -> bool:
  """hardwared, once, when the comma is about to power off for good: ask for
  the far end to go down with it. Returns at once: True when the request now
  waits for jetlinkd, which shutdown_pending() follows.

  A jetlink of API 1 from before the non-blocking power-off has no
  request_shutdown: it is asked the old way, blocking up to 25 s, so the
  Jetson is never left on for want of a method."""
  api = _api()
  ask = getattr(api, 'request_shutdown', None)
  if ask is None:
    api.shutdown(reason, 25.0)
    return False
  return ask(reason)


@_guarded(False)
def shutdown_pending() -> bool:
  """hardwared, every loop after request_shutdown(), until it puts DoShutdown:
  has jetlinkd still to take the request? A stat."""
  return getattr(_api(), 'shutdown_pending', lambda: False)()


@_guarded(False)
def should_extend_catalog() -> bool:
  """Should the big-model catalog carry the models newer catalogs list?
  Hardware, not the link setting: the model manager drops a pick its catalog
  does not list."""
  return _api().should_extend_catalog()


@_guarded(lambda catalog: catalog)
def extend_catalog(catalog: dict) -> dict:
  """The big-model catalog with those models folded in."""
  return _api().extend_catalog(catalog)
