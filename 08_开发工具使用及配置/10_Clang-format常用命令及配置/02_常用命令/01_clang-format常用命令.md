# clang-format 常用命令

[TOC]

## 概述

`clang-format` 是 LLVM 提供的代码格式化工具，支持 C/C++/Java/JavaScript/JSON/Objective-C/Protobuf/C# 等语言，能按照配置文件 (`.clang-format`) 自动统一代码风格

基本语法：

```bash
clang-format [options] [<file> ...]
```

不指定文件时从标准输入读取代码，格式化结果写到标准输出

## 格式化文件

### 输出到标准输出

默认将格式化结果打印到终端，不修改原文件：

```bash
clang-format main.c
```

### 原地修改文件 (-i)

`-i` 表示 inplace，直接修改原文件：

```bash
clang-format -i main.c

# 同时格式化多个文件
clang-format -i main.c util.c util.h
```

### 批量格式化

```bash
# 通配符
clang-format -i src/*.c src/*.h

# 递归格式化整个目录
find . -name '*.c' -o -name '*.h' | xargs clang-format -i

# 从文件中读取待处理文件列表 (每行一个路径)
clang-format -i --files=filelist.txt
```

加上 `--verbose` 可以打印正在处理的文件列表：

```bash
clang-format -i --verbose src/*.c
```

## 检查格式 (不修改文件)

### dry-run 模式 (-n)

`-n` (`--dry-run`) 只检查不做修改，若存在需要格式化的内容会输出警告：

```bash
clang-format -n main.c
```

### 配合 --Werror 用于 CI 检查

将格式警告升级为错误，退出码非 0，适合集成到 CI 或 git hook 中：

```bash
# 检查项目代码风格是否合规
find src -name '*.[ch]' | xargs clang-format -n --Werror

# 只报告前 N 个错误
clang-format -n --Werror --ferror-limit=10 main.c
```

## 指定代码风格 (--style)

### 使用预置风格

内置预置风格：`LLVM`、`GNU`、`Google`、`Chromium`、`Microsoft`、`Mozilla`、`WebKit`：

```bash
clang-format -i --style=Google main.c
```

### 使用 .clang-format 配置文件 (默认)

`--style=file` 是默认行为，会从源文件所在目录逐级向上查找 `.clang-format` 文件：

```bash
clang-format -i --style=file main.c

# 显式指定配置文件路径
clang-format -i --style=file:/path/to/.clang-format main.c
```

找不到配置文件时回退到 `--fallback-style` 指定的风格 (默认 `LLVM`)，设为 `none` 则跳过格式化：

```bash
clang-format -i --fallback-style=none main.c
```

### 命令行内联风格

用 `{key: value, ...}` 直接指定参数，可快速覆盖个别选项：

```bash
clang-format -i --style="{BasedOnStyle: llvm, IndentWidth: 8}" main.c

# 基于配置文件再覆盖个别项
clang-format -i --style="{BasedOnStyle: file, ColumnLimit: 120}" main.c
```

## 查看与生成配置

```bash
# 导出当前生效的完整配置 (含所有选项的默认值)
clang-format --dump-config > .clang-format

# 结合 --style 导出指定风格的完整配置
clang-format --dump-config --style=Google > .clang-format
```

## 格式化指定范围

只格式化文件中的部分代码，适合增量修改场景：

```bash
# 按行号范围 (1-based，闭区间)
clang-format --lines=10:20 main.c

# 多个范围可多次指定
clang-format --lines=10:20 --lines=30:40 main.c

# 按字节偏移 + 长度
clang-format --offset=100 --length=200 main.c
```

注意：范围参数只能用于单个输入文件；`--lines` 不能与 `--offset`/`--length` 同时使用；配合 `-i` 时仅原地修改指定范围

## 从标准输入格式化

```bash
# 直接管道输入
cat main.c | clang-format

# 用 --assume-filename 指定虚拟文件名，
# 用于确定语言类型和查找 .clang-format 配置文件
cat snippet.txt | clang-format --assume-filename=src/main.c
```

## 局部禁止格式化

在代码中用注释标记不想被格式化的区域：

```c
// clang-format off
int matrix[] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1,
};
// clang-format on
```

## 实用场景

### 只格式化 git 改动的部分

利用 `clang-format-diff` (随 clang-tools 安装) 只检查/格式化补丁涉及的行：

```bash
# 检查未提交改动是否符合格式 (只检查，-n 为 dry-run)
git diff -U0 | clang-format-diff -n

# 直接格式化最近一次提交改动的行 (-i 为原地修改，与 -n 互斥)
git diff -U0 HEAD~1 | clang-format-diff -i
```

### git pre-commit 钩子

`.git/hooks/pre-commit`：

```bash
#!/bin/sh
# 暂存区中有改动的 C/C++ 文件全部格式化后重新加入暂存区
git diff --cached --name-only --diff-filter=ACM | grep -E '\.(c|cpp|h|hpp)$' | \
    xargs -r clang-format -i
git diff --cached --name-only --diff-filter=ACM | grep -E '\.(c|cpp|h|hpp)$' | \
    xargs -r git add
```

### Makefile 集成

```makefile
SRCS := $(shell find src -name '*.[ch]')

format:
	clang-format -i $(SRCS)

check-format:
	clang-format -n --Werror $(SRCS)
```

## 常用选项速查

| 选项 | 说明 |
| --- | --- |
| `-i` | 原地修改文件 |
| `-n` / `--dry-run` | 只检查不修改 |
| `--Werror` | 格式警告升级为错误 |
| `--style=<风格>` | 指定风格：预置名 / `file` / `file:路径` / `{k:v,...}` |
| `--fallback-style=<风格>` | 找不到 `.clang-format` 时的回退风格 |
| `--dump-config` | 导出完整配置到标准输出 |
| `--lines=<起>:<止>` | 只格式化指定行范围 |
| `--offset=<N> --length=<N>` | 只格式化指定字节范围 |
| `--assume-filename=<文件>` | stdin 输入时指定虚拟文件名 |
| `--files=<列表文件>` | 从文件读取待处理文件清单 |
| `--sort-includes` | 覆盖配置中的 include 排序行为 |
| `--verbose` | 打印处理的文件列表 |
| `--version` | 查看版本 |
