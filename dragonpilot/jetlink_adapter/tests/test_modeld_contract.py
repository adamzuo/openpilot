"""Execute modeld's integration expressions without native camera/GPU bindings."""
import ast
from pathlib import Path
from types import SimpleNamespace
import unittest

ROOT = Path(__file__).resolve().parents[3]
TREE = ast.parse((ROOT / 'selfdrive/modeld/modeld.py').read_text())
MAIN = next(n for n in TREE.body if isinstance(n, ast.FunctionDef) and n.name == 'main')


def assignment(name):
  return next(n for n in ast.walk(MAIN)
              if isinstance(n, (ast.Assign, ast.AnnAssign))
              and any(isinstance(t, ast.Name) and t.id == name
                      for t in (n.targets if isinstance(n, ast.Assign) else [n.target])))


def evaluate(name, env):
  return eval(compile(ast.Expression(assignment(name).value), '<modeld>', 'eval'), env)


class ModeldContractTests(unittest.TestCase):
  def test_dropped_camera_frame_reaches_joining_fallback(self):
    for dropped in (0, 1, 10):
      self.assertFalse(evaluate('prepare_only', {'vipc_dropped_frames': dropped,
                                               'model': SimpleNamespace(joined=object())}))
      self.assertEqual(evaluate('prepare_only', {'vipc_dropped_frames': dropped,
                                                'model': SimpleNamespace()}), dropped > 0)

  def test_model_input_and_action_use_same_compensated_time(self):
    env = {'DT_MDL': .05, 'lat_delay': .3, 'long_delay': .5,
           'vec_desire': [], 'traffic_convention': [],
           'np': SimpleNamespace(array=lambda v, **kw: v, float32=float)}
    for name in ('frame_delay', 'action_delay', 'lat_action_t', 'long_action_t'):
      env[name] = evaluate(name, env)
    values = evaluate('inputs', env)['action_t']
    self.assertAlmostEqual(values[0], .375)
    self.assertAlmostEqual(values[1], .575)
    env.update(model_output=None, prev_action=None, v_ego=0, dp_lat_offset_cm=0,
               get_action_from_model=lambda output, previous, lat, lon, *args: [lat, lon])
    self.assertEqual(evaluate('action', env), values)

  def test_model_status_is_published_valid(self):
    env = {'messaging': SimpleNamespace(new_message=lambda service, valid=False: (service, valid))}
    self.assertEqual(evaluate('model_ext_send', env), ('modelExt', True))


if __name__ == '__main__':
  unittest.main()
