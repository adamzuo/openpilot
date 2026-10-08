import ast
import types
import unittest
from pathlib import Path
from dragonpilot.jetlink_adapter.panel import status_fields, STATUS_ROWS

class StatusViewsTest(unittest.TestCase):
  def link(self, **kw):
    values=dict(present=True, reason=None, model='next', active_model='previous', runnable=True,
                progress={'stage':'download','frac':.42,'msg':'fetching'})
    return types.SimpleNamespace(**(values|kw))
  def test_connection_separate_from_loading(self):
    f=status_fields(self.link())
    self.assertEqual(f['connection'],'CONNECTED')
    self.assertEqual(f['loading'],'download 42%')
    self.assertEqual(f['active'],'previous')
    self.assertEqual(f['model'],'next')
  def test_wait_has_no_fake_percent_and_drops_preserved(self):
    f=status_fields(self.link(progress={'stage':'waiting for jetlink','frac':0,'drops':3}))
    self.assertNotIn('%',f['loading'])
    self.assertIn('3 drops',f['message'])
  def test_disabled_error_unknown_and_nonfinite(self):
    self.assertEqual(status_fields(self.link(),False)['loading'],'OFF')
    self.assertEqual(status_fields(None)['connection'],'UNKNOWN')
    self.assertEqual(status_fields(self.link(reason='cable'))['connection'],'ERROR')
    self.assertEqual(status_fields(self.link(present=False))['connection'],'OFFLINE')
    self.assertNotIn('%',status_fields(self.link(progress={'stage':'build','frac':float('nan')}))['loading'])
  def test_both_settings_entries_wired(self):
    root=Path(__file__).resolve().parents[3]
    for sub, name in [('', 'settings_items'), ('mici/', 'mici_settings_items')]:
      tree=ast.parse((root/f'selfdrive/ui/{sub}layouts/settings/developer.py').read_text())
      self.assertTrue(any(isinstance(n,ast.Call) and isinstance(n.func,ast.Name) and n.func.id==name for n in ast.walk(tree)))
    self.assertEqual({k for k,_ in STATUS_ROWS},{'connection','model','active','loading','message','reason'})
