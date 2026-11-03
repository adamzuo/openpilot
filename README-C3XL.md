# C3XL 完整系统备份（2026-11-03）

## 设备当时状态
- 分支：`XL-devtest1029-tl-jetlink-tici`（来自 moumou758/openpilot）
- 主仓提交：`2f9c16f81`（C3XL: point opendbc at the BSM swap）
- AGNOS：**19.9**
- jetlink：**0.8.12**（iPhone A20 Pro / ANE 已连通）
- 大模型：Light Speed Model (October 29, 2026)
- 车型：TOYOTA_HIGHLANDER_TSS2

## 本机改动（只有一项）
**BSM 左右交换** —— 在 opendbc 子模块里（复刻机 CAN 左右信号相反）
补丁：`patches/0001-C3XL-swap-left-right-BSM-signals-clone-CAN-is-mirror.patch`

## 还原步骤
```bash
# 1) 装本分支（安装器填 adamzuo/openpilot XL-devtest1029-tl-jetlink-tici）
# 2) 等 AGNOS 19.9 + 编译完成
# 3) 打 BSM 补丁
cd /data/openpilot/opendbc_repo
git am /path/to/patches/0001-C3XL-swap-left-right-BSM-signals-clone-CAN-is-mirror.patch
cd /data/openpilot && git add opendbc_repo && git commit -m "C3XL: point opendbc at the BSM swap"
sudo systemctl restart comma
```

## 说明：LFS
本仓库的 `.lfsconfig` 指向 `huggingface.co/sunnypilot/sunnypilot-lfs`（匿��可读 ✓）。
本分支里的 LFS 文件以**指针**形式存储 ✓，clone 时 `git lfs pull` 会从 HF 拉真实内容 ✓。

## 关键参数快照
JetlinkLink=2, LongitudinalPersonality=0(Aggressive), ToyotaTSS2Smooth=1,
SmartCruiseControlVision=1, SmartCruiseControlMap=0, ToyotaEnforceStockLongitudinal=0
