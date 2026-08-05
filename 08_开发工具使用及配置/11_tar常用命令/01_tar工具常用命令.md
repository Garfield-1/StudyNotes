# tar 常用命令

[TOC]

## 概述

`tar`（tape archive）是 Linux 下最常用的打包/归档工具，本身只做"打包"而不压缩，需要配合压缩算法使用，通过不同的主选项和压缩参数组合格式：

| 后缀 | 压缩算法 | 说明 |
| ---- | -------- | ---- |
| `.tar` | 无 | 仅打包，不压缩 |
| `.tar.gz` / `.tgz` | gzip | 最通用，压缩速度快 |
| `.tar.bz2` | bzip2 | 压缩率高于 gzip，速度较慢 |
| `.tar.xz` | xz | 压缩率最高，速度最慢 |
| `.tar.zst` | zstd | 压缩率和速度的平衡点，较新系统支持 |

`tar` 会根据压缩参数（`-z`/`-j`/`-J`）或文件后缀自动选择压缩算法，较新版本（GNU tar 1.15+）解压时也可省略压缩参数，由 `tar` 自动识别

## 主选项（三选一）

```bash
tar -c 归档文件 源文件...        # create，创建归档
tar -x 归档文件                  # extract，解包
tar -t 归档文件                  # list，仅列出归档内容，不解包
```

## 常用辅助选项

```bash
-f 文件名        # 指定归档文件名，必须紧跟文件名（使用频率最高）
-v               # 显示处理过程（verbose）
-z               # 使用 gzip 压缩/解压（.tar.gz）
-j               # 使用 bzip2 压缩/解压（.tar.bz2）
-J               # 使用 xz 压缩/解压（.tar.xz）
-C 目录          # 解压时切换到指定目录（大写 C）
-r               # 向已有归档追加文件（仅支持未压缩的 .tar）
--delete 文件    # 从归档中删除指定文件（仅支持未压缩的 .tar）
--exclude 模式   # 打包时排除匹配的文件
-wildcards       # 匹配时使用通配符（配合 --exclude 等使用）
-k               # 解压时不覆盖已存在的文件
-p               # 保留文件权限信息
--totals         # 处理结束后输出总字节数
```

## 打包

| 命令作用 | 具体命令 | 产物类型 |
| -------- | -------- | -------- |
| 仅打包，不压缩 | `tar -cvf archive.tar 目录或文件` | `.tar` |
| 打包并 `gzip` 压缩 | `tar -czvf archive.tar.gz 目录或文件` | `.tar.gz` |
| 打包并 `bzip2` 压缩 | `tar -cjvf archive.tar.bz2 目录或文件` | `.tar.bz2` |
| 打包并 `xz` 压缩 | `tar -cJvf archive.tar.xz 目录或文件` | `.tar.xz` |
| 打包并 `zstd` 压缩 | `tar --zstd -cvf archive.tar.zst 目录或文件` | `.tar.zst` |
| 打包时排除 `.log` 文件 | `tar -cvf archive.tar --exclude="*.log" 目录` | `.tar` |
| 打包时排除指定子目录 | `tar -czvf archive.tar.gz --exclude=目录/.git 目录` | `.tar.gz` |

## 解包

| 命令作用 | 具体命令 | 输入文件类型 |
| -------- | -------- | ------------ |
| 解压到当前目录（自动识别压缩格式） | `tar -xvf archive.tar.gz` | `.tar.gz` |
| 显式指定 `gzip` 解压 | `tar -xzvf archive.tar.gz` | `.tar.gz` |
| 显式指定 `bzip2` 解压 | `tar -xjvf archive.tar.bz2` | `.tar.bz2` |
| 显式指定 `xz` 解压 | `tar -xJvf archive.tar.xz` | `.tar.xz` |
| 显式指定 `zstd` 解压 | `tar --zstd -xvf archive.tar.zst` | `.tar.zst` |
| 解压到指定目录（目录须已存在） | `tar -xvf archive.tar.gz -C /目标目录` | `.tar.gz` |
| 只解压归档中的某个文件 | `tar -xvf archive.tar 指定文件` | `.tar` |
| 按通配符解压匹配的文件 | `tar -xvf archive.tar --wildcards "*.conf"` | `.tar` |
| 不覆盖比归档中更新的文件 | `tar -xvf archive.tar.gz --keep-newer-files` | `.tar.gz` |

## 查看归档内容

```bash
tar -tvf archive.tar.gz                      # 列出归档内所有文件（含权限、大小）
tar -tvf archive.tar.gz | grep 关键字        # 查找归档中是否包含某文件
```

## 归档管理

以下操作仅对未压缩的 `.tar` 归档有效：

```bash
tar -rvf archive.tar 新文件                  # 向归档追加文件
tar --delete -f archive.tar 文件名           # 从归档中删除文件
tar -df archive.tar 目录                     # 对比归档与目录内容的差异
tar -Af archive.tar archive2.tar             # 将 archive2.tar 合并进 -f 指定的 archive.tar
```

## 实用技巧

### 排除 .git 目录打包源码

```bash
tar -czvf source.tar.gz --exclude="*/.git" 源码目录
```

### 打包时同时写入文件列表

```bash
tar -czvf archive.tar.gz 目录 | tee pack.log   # -v 的输出重定向保存
```

### 分卷压缩

`tar` 本身不支持分卷，打包后用 `split` 切割：

```bash
tar -czvf - 目录 | split -b 100M - archive.tar.gz.   # 每卷 100M
cat archive.tar.gz.* | tar -xzvf -                    # 合并后解压
```

### 保留权限和属主解包

```bash
sudo tar -xpvf archive.tar.gz -C /目标目录    # -p 保留权限，需 root 才能恢复属主
```

### 查看解压后占用空间

```bash
tar -tvf archive.tar.gz | awk '{sum += $3} END {print sum/1024/1024 " MB"}'
```
