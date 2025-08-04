# hostapd ctrl if模块梳理

`hostapd`的`ctrl i/f`（`control interface`，控制接口）模块主要作用是**为外部程序（如命令行工具hostapd_cli、GUI等）提供与hostapd守护进程进行交互的机制**，实现运行时控制和状态查询



## 对应源代码

`ctrl if`模块对应源代码是`./hostapd/ctrl_iface.c`和`./hostapd/ctrl_iface.h`

在`./hostapd/hostapd.conf`中可以找到相关的注释

```shell
# Interface for separate control program. If this is specified, hostapd
# will create this directory and a UNIX domain socket for listening to requests
# from external programs (CLI/GUI, etc.) for status information and
# configuration. The socket file will be named based on the interface name, so
# multiple hostapd processes/interfaces can be run at the same time if more
# than one interface is used.
# /var/run/hostapd is the recommended directory for sockets and by default,
# hostapd_cli will use it when trying to connect with hostapd.
ctrl_interface=/var/run/hostapd
```



## 配置句柄

`hostapd`通信时采用的方法是通过文件句柄来进行通信，句柄会存储在`hostapd.conf`中的`ctrl_interface`配置选项中，在读取配置文件时会将这个字段存入`struct hostapd_bss_config`中的`ctrl_interface`字段中用于后续的使用



## 使用命令

在启动hostapd主进程之后，输入

```shell
sudo ./hostapd_cli -i wlan0 help
```

`-i`指定端口，`help`查看使用帮助
