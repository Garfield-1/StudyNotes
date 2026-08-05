# tmux 常用命令

[TOC]

## 概述

`tmux` (Terminal Multiplexer) 是一款终端复用工具，主要功能：

- 会话持久化：断开 SSH 后程序继续在后台运行，重新连接后可恢复现场
- 窗口分屏：在一个终端内创建多个窗口和窗格
- 会话共享：多人可接入同一个会话

## 会话管理

### 创建会话

不加 `-s` 则使用数字默认命名：

```bash
tmux new -s 会话名
```

### 查看会话列表

```bash
tmux ls
```

### 接入会话

`attach` 可简写为 `a`：

```bash
tmux attach -t 会话名
```

### 关闭会话

不加 `-t` 时关闭最近使用的会话：

```bash
tmux kill-session -t 会话名

# 关闭除指定会话外的所有会话
tmux kill-session -a -t 会话名

# 关闭 tmux 服务器及所有会话
tmux kill-server
```

### 重命名会话

```bash
tmux rename-session -t 旧名 新名
```

### 判断会话是否存在

常用于脚本，会话存在则退出码为 0：

```bash
tmux has-session -t 会话名
```

## 前缀键

tmux 的快捷键都需要先按前缀键触发，默认前缀键为 `Ctrl+b`

按下前缀键后再按功能键即可执行对应操作，下文以 `Ctrl+b` 表示

## 会话快捷键

### 脱离当前会话

返回 Shell，会话仍在后台运行，可用 `tmux attach` 恢复：

```text
Ctrl+b d
```

### 会话列表菜单

以菜单方式显示会话列表并选择切换：

```text
Ctrl+b s
```

### 重命名当前会话

```text
Ctrl+b $
```

## 窗口操作

### 创建新窗口

```text
Ctrl+b c
```

### 切换窗口

```text
Ctrl+b n        # 下一个窗口
Ctrl+b p        # 上一个窗口
Ctrl+b l        # 上次使用的窗口
Ctrl+b 数字     # 对应编号的窗口, 如 Ctrl+b 1
Ctrl+b w        # 以菜单方式显示窗口列表并选择切换
```

### 重命名当前窗口

```text
Ctrl+b ,
```

### 关闭当前窗口

需二次确认：

```text
Ctrl+b &
```

### 窗格拆分为独立窗口

```text
Ctrl+b !
```

## 窗格操作

### 分割窗格

```text
Ctrl+b %        # 左右分割 (垂直分屏)
Ctrl+b "        # 上下分割 (水平分屏)
```

### 移动窗格焦点

```text
Ctrl+b 方向键   # 向对应方向移动焦点
Ctrl+b o        # 切换到下一个窗格
Ctrl+b ;        # 切换到上一个活动窗格
Ctrl+b q        # 显示窗格编号, 按下对应数字可跳转
```

### 最大化/还原当前窗格

```text
Ctrl+b z
```

### 关闭当前窗格

需二次确认：

```text
Ctrl+b x
```

### 切换内置布局

多窗格时可循环尝试不同排列方式：

```text
Ctrl+b 空格
```

### 调整窗格大小

可连续按：

```text
Ctrl+b Ctrl+方向键
```

### 交换窗格位置

当前窗格与上一个/下一个窗格交换位置：

```text
Ctrl+b {
Ctrl+b }
```

## 复制模式

### 进入复制模式

可用方向键或 `PgUp`/`PgDn` 翻页查看历史输出，`q` 退出：

```text
Ctrl+b [
```

### 粘贴最近复制的内容

```text
Ctrl+b ]
```

### vim 按键复制模式

需在配置文件中开启 `mode-keys vi`，进入后支持 `h/j/k/l` 移动、`v` 选择、`y` 复制、`/` 搜索、`g/G` 跳转顶部/底部：

```text
Ctrl+b v
```

## 命令行模式

按下 `Ctrl+b` 后再按 `:` 进入命令行模式，可执行 tmux 命令，例如：

```text
:swap-window -t 0       # 将当前窗口移动到 0 号位置
:split-window -h        # 左右分割窗格
:resize-pane -L 10      # 当前窗格向左扩展 10 格
```

## 其他常用快捷键

### 显示快捷键帮助

```text
Ctrl+b ?
```

### 显示时钟

按 `Enter` 返回终端：

```text
Ctrl+b t
```

## 配置文件

配置文件默认路径为 `~/.tmux.conf`，修改后执行以下命令使配置生效：

```bash
tmux source-file ~/.tmux.conf
```

## 鼠标支持

tmux 默认不支持鼠标操作，在配置文件中添加以下内容开启：

```bash
set -g mouse on
```

开启后支持鼠标点击切换窗格、拖动调整窗格大小、滚轮翻页查看历史输出
