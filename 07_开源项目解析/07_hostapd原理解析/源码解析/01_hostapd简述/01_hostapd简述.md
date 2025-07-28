# hostapd简述

> 笔者注：当前主流的hostapd实现是采用的Jouni Malinen的版本，后文中也是也是围绕这个版本来展开的

## hostapd是什么

这里附上不同的解读

[wikipedia：]: https://en.wikipedia.org/wiki/Hostapd

> **hostapd** (host access point daemon) is a user space daemon software enabling a network interface cardto act as an access point and authentication server. There are three implementations: Jouni Malinen's hostapd, OpenBSD's hostapd and Devicescape's hostapd.
>
> Jouni Malinen's hostapd is a user space daemon for access point and authentication servers. It can be used to create a wireless hotspot using a Linux computer.It implements IEEE 802.11access point management, IEEE 802.1X/WPA/WPA2/EAP Authenticators, RADIUSclient, EAP server, and RADIUS authentication server. The current version supports Linux(Host AP, MadWifi,Prism54) and some of the drivers which use the kernel's mac80211 subsystem), QNX, FreeBSD (net80211), and DragonFlyBSD.

中文翻译：

>**hostapd**（主机接入点守护程序）是一个用户空间守护程序软件，使网络接口卡充当接入点和身份验证服务器。有三种实现：Jouni Malinen的hostapd， OpenBSD的hostapd和Devicescape的hostapd
>
>Jouni Malinen的hostapd是用于接入点和身份验证服务器的用户空间守护进程。它可以用于使用Linux计算机创建无线热点。它实现了IEEE 802.11接入点管理、IEEE 802.1X/WPA/WPA2/EAP认证器、RADIUSclient、EAP服务器和RADIUS认证服务器。当前版本支持Linux（Host AP, MadWifi,Prism54）和一些使用内核mac80211子系统的驱动程序)，QNX, FreeBSD （net80211）和DragonFlyBSD

**`hostapd`源码`README`**

> Originally, hostapd was an optional user space component for Host AP driver. It adds more features to the basic IEEE 802.11 management included in the kernel driver: using external RADIUS authentication
> server for MAC address based access control, IEEE 802.1X Authenticator and dynamic WEP keying, RADIUS accounting, WPA/WPA2 (IEEE 802.11i/RSN) Authenticator and dynamic TKIP/CCMP keying.
>
> The current version includes support for other drivers, an integrated EAP server (i.e., allow full authentication without requiring an external RADIUS authentication server), and RADIUS authentication server for EAP authentication.

中文翻译：

> 最初，hostapd是主机AP驱动程序的可选用户空间组件。它为内核驱动程序中包含的基本IEEE 802.11管理添加了更多特性：使用外部RADIUS身份验证
> 基于MAC地址的访问控制服务器，IEEE 802.1X认证器和动态WEP密钥，RADIUS计费，WPA/WPA2 （IEEE 802.11i/RSN）认证器和动态TKIP/CCMP密钥。
>
> 当前版本包括对其他驱动程序的支持，一个集成的EAP服务器（即，允许完整的身份验证而不需要外部RADIUS认证服务器），以及用于EAP身份验证的RADIUS认证服务器。

## hostapd可以做什么

**使用场景**

- 无线路由器：`hostapd` 是许多开源路由器固件（如 `OpenWrt`）中实现无线功能的核心组件
- 无线热点：在 `Linux` 系统上使用 `hostapd` 可以将笔记本电脑或树莓派配置为无线热点
- 企业无线网络：通过 `RADIUS` 服务器和 `hostapd` 配置企业级无线网络

## 工作原理

`hostapd` 通过与无线网卡驱动交互，控制无线网卡的工作模式，将其切换为 `AP` 模式（Access Point Mode）并通过配置文件定义无线网络的参数（如 `SSID`、密码、信道等）
