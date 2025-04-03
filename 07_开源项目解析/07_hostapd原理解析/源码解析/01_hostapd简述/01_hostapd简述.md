# hostapd简述

## hostapd是什么

`hostapd` 是`host access point daemon`的缩写。它是运行在用户空间的守护程序。它通常在后台运行，用于将无线网络卡充当访问点（AP），并作为控制身份验证的后端组件。曾经有三个不同版本的`hostapd`，分别是`Jouni Malinen`版、`OpenBSD`版和`OWL`（ `Open Wireless Linux` ）版。第三种版本的`hostapd`未曾有稳定版本发布，目前整个项目已经终止。`Jouni Malinen`版的`hostapd`是目前应用最为广泛的一个版本，安卓手机、基于`OpenWrt`的无线`AP`、`Linux PC`等设备多使用该版本的`hostapd`来创建无线接入点和管理无线设备的接入。



## hostapd可以做什么

**使用场景**

- 无线路由器：`hostapd` 是许多开源路由器固件（如 `OpenWrt`）中实现无线功能的核心组件。
- 无线热点：在 `Linux` 系统上使用 `hostapd` 可以将笔记本电脑或树莓派配置为无线热点。
- 企业无线网络：通过 `RADIUS` 服务器和 `hostapd` 配置企业级无线网络。



## 工作原理

`hostapd` 通过与无线网卡驱动交互，控制无线网卡的工作模式，将其切换为 `AP` 模式（Access Point Mode）并通过配置文件定义无线网络的参数（如 `SSID`、密码、信道等）。

