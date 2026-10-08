"""Guard the dp0111pre engagement gate across upstream model handovers."""
import sys
import types
import unittest
from unittest.mock import patch
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[3]))
from dragonpilot.jetlink_adapter.model import FullModel

class ProvingTest(unittest.TestCase):
  def test_initial_frames_rejoin_fallback_and_skipped_frames(self):
    small = types.SimpleNamespace(vision_input_names=[], run=lambda *args: None)
    joined = types.SimpleNamespace(handovers=0, chestnut=False, run=lambda *args: {})
    model = FullModel(small, joined)
    state = types.ModuleType('jetlink.openpilot.model_state')
    state.PROVING_FRAMES = 20
    with patch.dict(sys.modules, {'jetlink.openpilot.model_state': state}):
      self.assertFalse(model.proving)
      joined.handovers = 1
      joined.chestnut = True
      for _ in range(20):
        model.run({}, {}, {}, False)
        self.assertTrue(model.proving)
      model.run({}, {}, {}, True)
      self.assertTrue(model.proving)
      model.run({}, {}, {}, False)
      self.assertFalse(model.proving)
      joined.handovers = 2
      joined.chestnut = False
      model.run({}, {}, {}, False)
      self.assertFalse(model.proving)
      joined.handovers = 3
      joined.chestnut = True
      model.run({}, {}, {}, False)
      self.assertTrue(model.proving)
