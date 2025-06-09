# Linux内核编译

> 参考文档：
>
> [Release v5.4 · torvalds/linux](https://github.com/torvalds/linux/releases/tag/v5.4)



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
