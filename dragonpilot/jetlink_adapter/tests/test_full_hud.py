"""Run actual HUD entry methods and panel draw calls with mocked native bindings.
Not a raylib/GPU or on-device rendering test.
"""
import ast
import importlib.util
import sys
import types
import unittest
from pathlib import Path
from unittest.mock import patch
from PIL import Image

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))
from dragonpilot.jetlink_adapter import panel


class Rect:
  def __init__(self, x, y, width, height):
    self.x, self.y, self.width, self.height = x, y, width, height


class Color:
  def __init__(self, r, g, b, a=255):
    self.r, self.g, self.b, self.a = r, g, b, a


class SM:
  def __init__(self, state='full: running', proving=False):
    self.alive = {'modelExt': True}; self.valid = {'modelExt': True}; self.recv_frame = {'modelExt': 20}
    self.data = types.SimpleNamespace(jetlinkState=state, jetlinkProving=proving)
  def __getitem__(self, key): return self.data


def module(name, **fields):
  m = types.ModuleType(name)
  m.__dict__.update(fields)
  return m


def method(path, name, globals_):
  tree = ast.parse((ROOT/path).read_text())
  cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == 'HudRenderer')
  fn = next(n for n in cls.body if isinstance(n, ast.FunctionDef) and n.name == name)
  fn.returns = None
  for arg in fn.args.args: arg.annotation = None
  exec(compile(ast.Module(body=[fn], type_ignores=[]), str(path), 'exec'), globals_)
  return globals_[name]


class HUDTests(unittest.TestCase):
  def setUp(self):
    for name in ('checked', 'enabled', 'link'):
      if hasattr(panel.draw_status, name): delattr(panel.draw_status, name)
    self.calls=[]; self.textures=[]; self.mode=1
    self.sm=SM()
    self.link=types.SimpleNamespace(enabled=True, reason=None, present=True)
    self.ui=types.SimpleNamespace(sm=self.sm, started_frame=10)
    def texture(path, width, height):
      file=ROOT/'selfdrive/assets'/path
      with Image.open(file) as im:
        im.verify()
      self.textures.append(path)
      return types.SimpleNamespace(width=width, height=height, path=path)
    def text(font, value, pos, size, spacing, color):
      width=sum(size if ord(c)>127 else size*.6 for c in value)
      self.calls.append(('text', (pos.x,pos.y,width,size),value))
    def measure(font, value, size, spacing):
      return types.SimpleNamespace(x=sum(size if ord(c)>127 else size*.6 for c in value), y=size)
    self.rl=module('pyray', Rectangle=Rect, Vector2=lambda x,y:types.SimpleNamespace(x=x,y=y),Color=Color,WHITE=Color(255,255,255),
      draw_rectangle_rec=lambda r,c:self.calls.append(('bg',(r.x,r.y,r.width,r.height),None)),
      draw_texture_ex=lambda t,p,r,s,c:self.calls.append(('icon',(p.x,p.y,t.width*s,t.height*s),(t.path,c.a))),
      draw_text_ex=text,measure_text_ex=measure,draw_rectangle_gradient_v=lambda *a:None)
    params=lambda:types.SimpleNamespace(get=lambda k:self.mode)
    app=types.SimpleNamespace(texture=texture,font=lambda _:None)
    mods={'pyray':self.rl,
      'openpilot.selfdrive.ui.ui_state':module('ui_state',ui_state=self.ui),
      'openpilot.system.ui.lib.application':module('application',gui_app=app,FontWeight=types.SimpleNamespace(MEDIUM=0)),
      'openpilot.common.params':module('params',Params=params),
      'openpilot.system.ui.lib.text_measure':module('text_measure',measure_text_cached=lambda f,t,s:measure(f,t,s,0))}
    self.patch=patch.dict(sys.modules,mods);self.patch.start()
    self.status_patch=patch('dragonpilot.jetlink_adapter.status',lambda:self.link);self.status_patch.start()
  def tearDown(self):
    self.status_patch.stop();self.patch.stop()
  def check_bounds(self,rect,variant):
    self.assertTrue(self.textures)
    gx,gy,gw,gh=panel.geometry(rect,variant)
    for _,(x,y,w,h),_ in self.calls:
      self.assertGreaterEqual(x,gx);self.assertGreaterEqual(y,gy)
      self.assertLessEqual(x+w,gx+gw+.01);self.assertLessEqual(y+h,gy+gh+.01)
      self.assertLessEqual(x+w,rect.x+rect.width);self.assertLessEqual(y+h,rect.y+rect.height)
    if variant=='tici': self.assertLess(gy+gh,rect.y+rect.height-78)
    else:
      self.assertGreater(gx,rect.x+rect.width/2+22.5)
      self.assertLess(gx+gw,rect.x+rect.width-60)
  def run_hud(self,variant):
    relative='selfdrive/ui/'+('mici/' if variant=='mici' else '')+'onroad/hud_renderer.py'
    gl={'rl':self.rl,'UI_CONFIG':types.SimpleNamespace(header_height=300,border_size=30,button_size=192),
        'COLORS':types.SimpleNamespace(HEADER_GRADIENT_START=None,HEADER_GRADIENT_END=None)}
    fake=types.SimpleNamespace(is_cruise_available=False,is_cruise_set=False,_can_draw_top_icons=True,
                              _exp_button=types.SimpleNamespace(render=lambda _:None))
    for n in ('_draw_current_speed','_draw_tdx_info','_draw_performance_info','_draw_edge_warnings','_draw_lead_info'):
      setattr(fake,n,lambda _:None)
    if variant=='tici':fake.draw_jetlink=types.MethodType(method(relative,'draw_jetlink',gl),fake)
    rect=Rect(30,30,2100,1020) if variant=='tici' else Rect(0,0,476,240)
    method(relative,'_render',gl)(fake,rect)
    self.check_bounds(rect,variant)
  def test_tici_actual_hud_method(self): self.run_hud('tici')
  def test_mici_actual_hud_method(self): self.run_hud('mici')
  def test_all_states_both_viewports(self):
    for variant,rect in [('tici',Rect(30,30,2100,1020)),('tici',Rect(320,30,1500,1020)),('mici',Rect(0,0,476,240))]:
      for state,proving,filename in [('full: running',False,'chestnut_green.png'),('full: running',True,'chestnut.png'),
          ('full: joining',False,'chestnut.png'),('full: ready',False,'chestnut.png'),('full: unavailable',False,'chestnut_orange.png'),
          ('local',False,'chestnut_orange.png')]:
        self.calls.clear();self.sm.data.jetlinkState=state;self.sm.data.jetlinkProving=proving
        panel.draw_status(rect,variant)
        self.assertTrue(self.textures[-1].endswith(filename));self.check_bounds(rect,variant)
  def test_off_no_draw(self):
    self.mode=0;panel.draw_status(Rect(0,0,476,240),'mici');self.assertFalse(self.calls)
  def test_mici_alert_no_draw(self):
    panel.draw_status(Rect(0,0,476,240),'mici',show=False);self.assertFalse(self.calls)
  def test_stale_never_green(self):
    for missing in ['alive','valid','old_drive']:
      self.calls.clear()
      self.sm.alive['modelExt']=missing!='alive';self.sm.valid['modelExt']=missing!='valid'
      self.sm.recv_frame['modelExt']=0 if missing=='old_drive' else 20
      panel.draw_status(Rect(0,0,476,240),'mici')
      self.assertTrue(self.textures[-1].endswith('chestnut_orange.png'))
  def test_tici_hidden_hud_call_path(self):
    tree=ast.parse((ROOT/'selfdrive/ui/onroad/augmented_road_view.py').read_text())
    branches=[n for n in ast.walk(tree) if isinstance(n,ast.If) and ast.unparse(n.test)=='not hide_hud']
    self.assertTrue(any('draw_jetlink' in ast.unparse(ast.Module(body=n.orelse,type_ignores=[])) for n in branches))
  def test_no_auxiliary_runtime(self):
    self.assertFalse((ROOT/'dragonpilot/jetlink_adapter/visiond.py').exists())
    for rel in ['system/manager/process_config.py','selfdrive/modeld/modeld.py','common/params_keys.h','dragonpilot/jetlink_adapter/panel.py']:
      text=(ROOT/rel).read_text()
      for forbidden in ('JetlinkFullModel','JetlinkVisionHz','JetlinkVisionStatus','jetlinkvisiond','Auxiliary'):
        self.assertNotIn(forbidden,text)

if __name__=='__main__': unittest.main()
