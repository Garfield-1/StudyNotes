# Linux内核编译

> 参考文档：
>
> [Release v5.4 · torvalds/linux](https://github.com/torvalds/linux/releases/tag/v5.4)
>
> [编译内核模块 （Bosanski） - ArchWiki](https://wiki.archlinux.org/title/Compile_kernel_module_(Bosanski))



## 软件环境

`Linux`版本`V5.4`

`WSL2 ubuntu 20.04`

这里说明一下，编译`Linux 5.4`版本如果使用更高版本的`ubuntu`去编译，则需要单独安装低版本的工具链。建议直接使用`ubuntu 20.04`版本



## 编译依赖

```shell
sudo apt update
sudo apt install build-essential
sudo apt install libncurses5-dev libncursesw5-dev
sudo apt install flex
sudo apt install bison
sudo apt install libelf-dev libssl-dev
```



## 编译步骤

1. 在`linux`源码根目录执行`make menuconfig`生成`.config`文件
2. 执行`make`，如果出现`Kernel: arch/x86/boot/bzImage is ready`这种字样则说明编译成功



## 从makefile文件看编译流程

内核涉及到的编译文件主要如下

`Linux`内核编译时，主要涉及以下几类 `Makefile` 文件：

1. **顶层 Makefile**
   位于内核源码根目录（如 `Makefile`），负责整体编译流程的控制和全局变量、规则的定义。
2. **子目录 Makefile**
   各源码子目录（如 `arch`, `drivers`, `fs`, `net`, `mm`, `lib`等）下的 `Makefile`，负责本目录下源码的编译规则和依赖管理。例如：
   - `Makefile`：架构相关的编译规则
   - `Makefile`：驱动相关的编译规则
3. **Kbuild 文件**
   许多源码目录下有`Kbuild`或 `Kbuild.include` 文件（有时直接写在 Makefile 中），用于指定哪些源文件需要编译、如何编译。
4. **scripts/ 目录下的 Makefile**
   如 `scripts/Makefile.*`，用于辅助工具、配置、模块等的编译和处理。
5. **配置相关 Makefile**
   如 `Makefile`，用于处理内核配置（menuconfig、oldconfig等）。

**总结**：

- 根目录 `Makefile` 控制全局和递归编译
- 各子目录 `Makefile/Kbuild` 负责本目录源码编译
- `scripts/` 和 `config` 相关 `Makefile` 处理配置和辅助工具
- `arch/$(ARCH)/Makefile` 负责架构相关细节



## 根目录makefile

### 清理编译产物

如果我们在根目录下执行make clean动作会打印如下日志

可以看到编译产物主要是指定的CPU平台下的工具链、依赖库、驱动程序等

```shell
pi@XX-00070:~/code/linux-5.4$ make clean
  CLEAN   arch/x86/entry/vdso
  CLEAN   arch/x86/kernel/cpu
  CLEAN   arch/x86/kernel
  CLEAN   arch/x86/realmode/rm
  CLEAN   arch/x86/lib
  CLEAN   certs
  CLEAN   drivers/firmware/efi/libstub
  CLEAN   drivers/scsi
  CLEAN   drivers/tty/vt
  CLEAN   drivers/video/logo
  CLEAN   lib
  CLEAN   net/wireless
  CLEAN   security/selinux
  CLEAN   usr
  CLEAN   arch/x86/boot/compressed
  CLEAN   arch/x86/boot
  CLEAN   arch/x86/tools
  CLEAN   modules.builtin.modinfo
```

### 执行编译动作

1. **初始化和版本定义**:

   - 设置内核的 `VERSION`, `PATCHLEVEL`, `SUBLEVEL`, `EXTRAVERSION` 和 `NAME`
   - 定义默认目标 `_all`

2. **引入关键编译配置脚本:**

   ```shell
   include scripts/Kbuild.include
   include scripts/subarch.include
   ```

3. **设置CPU平台，引入对应的配置和编译脚本**





