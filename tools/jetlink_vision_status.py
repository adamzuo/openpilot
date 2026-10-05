#!/usr/bin/env python3
"""Read-only diagnostic: actual lead source published to radard, once per second."""
import argparse
import json
import time

from cereal import messaging


def main():
  parser = argparse.ArgumentParser(description=__doc__)
  parser.add_argument('--count', type=int, default=0, help='number of reports; 0 keeps running')
  args = parser.parse_args()
  sm = messaging.SubMaster(['modelV2', 'radarState'], poll='modelV2')
  last_report, reports = 0., 0
  while args.count == 0 or reports < args.count:
    sm.update(1000)
    now = time.monotonic()
    if not sm.updated['modelV2'] or now-last_report < 1.:
      continue
    last_report = now
    m = sm['modelV2']
    info = m.jetlinkVision
    lead = m.leadsV3[0] if len(m.leadsV3) else None
    radar = sm['radarState'].leadOne
    print(json.dumps({'source': str(info.source), 'reason': info.reason, 'target_hz': info.targetHz,
                      'max_age_ms': round(info.maxAgeSeconds*1000, 1), 'guard_active': info.guardActive,
                      'phone_frame': info.frameId, 'phone_age_ms': round(info.ageSeconds*1000, 1),
                      'camera_frame': m.frameId, 'model_valid': sm.valid['modelV2'],
                      'lead_probability': lead.prob if lead else None,
                      'vision_x_m': lead.x[0] if lead and len(lead.x) else None,
                      'radar_alive': sm.alive['radarState'], 'radar_lead': radar.status,
                      'dRel_m': radar.dRel, 'vRel_mps': radar.vRel}, ensure_ascii=False), flush=True)
    reports += 1


if __name__ == '__main__':
  main()
