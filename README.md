# C3XL 系统备份（XL-tl-new-tici + BSM 补丁）

## 设备状态快照（2026-10-31）
- 系统分支：`XL-tl-new-tici` @ moumou758/openpilot（96f30e6 "openpilot vtesla"）
- AGNOS：**19.9**
- panda：在线，固件被此系统重刷为 `DEV-unknown-DEBUG`
- 车型：TOYOTA HIGHLANDER TSS2（支持 ✓）
- 校准：已恢复（`backup/CalibrationParams.bin`，md5 d4061854…）
- 本机改动：**只有一项** —— BSM 左右交换（见 `0001-BSM-swap.patch`）

## 完整系统怎么复现
1. 用 `https://flash13.mr-one.cn` 刷厂商 AGNOS（或任意可启动系统）
2. 安装器填 `moumou758/XL-tl-new-tici`
3. 它会自动升到 AGNOS 19.9
4. 打补丁：`cd /data/openpilot && git am 0001-BSM-swap.patch`
5. 恢复校准：
   ```bash
   scp backup/CalibrationParams.bin comma@<IP>:/tmp/
   ssh comma@<IP> 'sudo cp /tmp/CalibrationParams /data/params/d/ && sudo chown comma:comma /data/params/d/CalibrationParams && sudo chmod 600 /data/params/d/CalibrationParams && sudo systemctl restart comma'
   ```
6. 校准校验：`md5sum /data/params/d/CalibrationParams` → `d4061854fdf148873b549285e7c5de8a`

## 为什么不直接推送完整代码
moumou758/openpilot 仓库 **1.7 GB**（单提交导出），
推送完整分支需要上传 1.7GB ✗ 所以这里只保存「差异 + 恢复要素」。
需要完整镜像时可另行推送（见讨论）。
