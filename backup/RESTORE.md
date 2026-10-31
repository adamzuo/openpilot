# dpegpu0120261031 恢复说明

## 备份内容
| 文件 | 说明 |
|---|---|
| `params.tgz` | 121 个参数（含 53 个 dp_*）|
| `CalibrationParams.bin` | 校准 176B（md5 d4061854…）|
| `agnos-18.4.json` | AGNOS 18.4 分区清单（7 分区 4.74GB，来自 comma CDN）|
| `env-snapshot.txt` | 代码/系统/panda/关键参数快照 |
| `manifest.txt` | 备份时 manifest |

## 恢复流程（已实测，约 30 分钟）
1) 用 **https://flash13.mr-one.cn** 刷厂商 AGNOS 19.7（恢复定制 xbl）
2) 屏幕安装器填 `adamzuo/dpegpu01` → 会自动升到 **AGNOS 18.4**
3) 恢复参数：
   ```bash
   scp -O params.tgz comma@<IP>:/tmp/
   ssh comma@<IP> 'cd /data && sudo tar xzf /tmp/params.tgz && sudo chown -R comma:comma /data/params/d && sudo chmod 600 /data/params/d/* && sudo systemctl restart comma'
   ```
4) 校准（若 tar 里硬链接异常，单独写）：
   ```bash
   scp -O CalibrationParams.bin comma@<IP>:/tmp/CalibrationParams
   ssh comma@<IP> 'sudo cp /tmp/CalibrationParams /data/params/d/CalibrationParams && sudo chown comma:comma /data/params/d/CalibrationParams && sudo chmod 600 /data/params/d/CalibrationParams'
   ```
5) 检查：`cat /data/params/d/LongitudinalPersonality` 等，见 env-snapshot.txt
