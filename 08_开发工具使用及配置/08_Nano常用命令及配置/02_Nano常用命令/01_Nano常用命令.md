# nano 常用命令

[TOC]

## 概述

`nano` 是一款轻量级终端文本编辑器，开箱即用，无需像 `vim` 一样区分模式，打开即可直接编辑，常用于服务器、嵌入式设备（如 `OpenWrt`）上快速修改配置文件

`nano` 界面底部的快捷键提示中，`^` 表示 `Ctrl` 键，`M-` 表示 `Alt` 键

本文以 `nano 8.x` 的默认键绑定为准（`8.0` 起 `Ctrl+F`/`Ctrl+B` 改为发起搜索，不再是光标左右移动）

## 基本启动命令

```bash
nano 文件名                      # 打开或新建文件
nano +行号 文件名                # 打开文件并跳转到指定行
nano -c 文件名                   # 显示光标所在行号、列号
nano -l 文件名                   # 显示行号
nano -m 文件名                   # 开启鼠标支持
nano -v 文件名                   # 以只读模式打开
nano -E 文件名                   # Tab 键转换为空格
nano -w 文件名                   # 关闭长行自动换行（编辑长 URL/路径时常用）
nano +/字符串 文件名             # 打开文件并跳转到字符串首次出现的位置
```

## 快捷键

### 保存与退出

```bash
Ctrl+o          # 保存文件（按 Enter 确认文件名）
Ctrl+x          # 退出，未保存时会提示是否保存
```

### 光标移动

```bash
Ctrl+a          # 移动到行首
Ctrl+e          # 移动到行尾
方向键          # 上下左右移动光标
Ctrl+p          # 向上移动一行
Ctrl+n          # 向下移动一行
Ctrl+y          # 向上翻一页
Ctrl+v          # 向下翻一页
Ctrl+_          # 跳转到指定行号、列号
Alt+/           # 跳转到文件末尾
Alt+\           # 跳转到文件开头
```

### 删除与撤销

```bash
Ctrl+k          # 剪切（删除）当前行
Ctrl+u          # 粘贴剪切的内容
Ctrl+h          # 向前删除一个字符（Backspace）
Ctrl+d          # 向后删除一个字符（Delete）
Ctrl+6          # 设置选区标记，配合方向键选择文本
Alt+6           # 复制选区文本到剪切缓冲区
Alt+u           # 撤销上一步操作
Alt+e           # 重做上一步操作
```

### 查找与替换

```bash
Ctrl+w          # 搜索字符串
Ctrl+f          # 发起向前搜索（nano 8.0+）
Ctrl+b          # 发起向后搜索（nano 8.0+）
Ctrl+\          # 查找并替换字符串
```

搜索后可按 `Ctrl+r` 切换是否忽略大小写，按 `Alt+f`/`Alt+b` 查找下一个/上一个匹配项

`nano 8.0` 之前的版本中 `Ctrl+f`/`Ctrl+b` 为光标右移/左移，搜索由 `Ctrl+w` 发起

### 其他操作

```bash
Ctrl+g          # 显示帮助信息
Ctrl+c          # 显示当前光标所在行号、列号
Ctrl+t          # 调用外部命令执行
Ctrl+l          # 刷新屏幕
Ctrl+j          # 两端对齐当前段落
Alt+3           # 注释/取消注释当前行（根据文件类型自动选择注释符）
Alt+n           # 切换显示行号
Alt+m           # 切换鼠标支持
Alt+s           # 切换长行软换行
Alt+x           # 切换底部帮助栏显示
```

## 配置文件

配置文件默认路径为 `~/.nanorc`，常用配置项：

```bash
set linenumbers    # 显示行号
set mouse          # 开启鼠标支持
set tabsize 4      # Tab 显示为 4 个空格
set tabstospaces   # Tab 键转换为空格
set autoindent     # 自动缩进
set softwrap       # 长行软换行显示
set constantshow   # 始终显示光标位置（行号、列号）
```

系统级配置位于 `/etc/nanorc`，语法与用户配置相同，可配置语法高亮规则（`include` 指令）

## 语法高亮

`nano` 自带多语言语法高亮规则，默认多数发行版已启用，可在配置文件中通过 `include` 手动引入：

```bash
include "/usr/share/nano/c.nanorc"
include "/usr/share/nano/sh.nanorc"
include "/usr/share/nano/python.nanorc"
```
