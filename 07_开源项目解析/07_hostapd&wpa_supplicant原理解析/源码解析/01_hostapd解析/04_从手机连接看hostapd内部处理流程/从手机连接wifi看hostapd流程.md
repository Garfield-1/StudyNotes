# 从手机连接wifi看hostapd流程

[TOC]

## wifii连接过程

在关闭`wifi`的密码，设置为开放连接，直接使用手机连接路由器`wifi`，读取`hostapd`的日志如下

```shell
Tue Apr 22 21:52:30 2025 daemon.info hostapd: wlan1: STA xx:xx:xx:xx:cc:49 IEEE 802.11: authenticated
Tue Apr 22 21:52:30 2025 daemon.info hostapd: wlan1: STA xx:xx:xx:xx:cc:49 IEEE 802.11: associated (aid 1)
Tue Apr 22 21:52:30 2025 daemon.info hostapd: wlan1: STA xx:xx:xx:xx:cc:49 RADIUS: starting accounting session A8FF112BFD77B744
```

开启`wifi`密码，可以看到开启`wifi`密码后会明显的多出一条关于`wpa`的日志

```shell
Tue Apr 22 23:13:40 2025 daemon.info hostapd: wlan0: STA xx:xx:xx:xx:0a:fb IEEE 802.11: authenticated
Tue Apr 22 23:13:40 2025 daemon.info hostapd: wlan0: STA xx:xx:xx:xx:0a:fb IEEE 802.11: associated (aid 1)
Tue Apr 22 23:13:40 2025 daemon.info hostapd: wlan0: STA xx:xx:xx:xx:0a:fb RADIUS: starting accounting session D7F2785787303561
Tue Apr 22 23:13:40 2025 daemon.info hostapd: wlan0: STA xx:xx:xx:xx:0a:fb WPA: pairwise key handshake completed (RSN)
```

**从syslog中看**

这里结合源码可以看出

第一条日志对应的函数是：`handle_auth_cb`

从这里切入查看代码的函数调用栈如下

```c++
//hostapd/main.c
//main函数中注册了hostapd_wpa_event函数作为
main
    ->hostapd_wpa_event
        ->hostapd_mgmt_tx_cb
            //hostapd中的发送管理帧的状态机
            ->ieee802_11_mgmt_cb
                ->handle_auth_cb
                ->handle_assoc_cb
```

从代码调用栈上来是`wifi`连接后触发了`hostapd`中对于管理帧处理的流程，这段逻辑开始来自`netlink`事件上报，然后触发`hostapd`中的状态机中，接着状态机处理管理帧报文时打印了这些日志

在源码中新增一些日志后可以分析道更加详细的日志，看到更加详细函数调用栈

```c
//初始化时注册了处理netlink事件的回调
wpa_driver_nl80211_init_nl_global
    //处理netlink全局事件
    ->process_global_event
        ->do_process_drv_event
            //处理NL80211_CMD_FRAME_TX_STATUS事件
            ->mlme_event
                ->mlme_event_mgmt_tx_status
                    //这里只是一个函数指针，最终是在main函数中注册
                    ->wpa_supplicant_event
```

这里的`wpa_supplicant_event`实际上是在`main`函数中注册的函数回调入口

## netlink初始化注册

函数调用栈

```c
//hostapd/main.c
main
    ->hostapd_driver_init
        ->wpa_drivers[i]->global_init

//src/drivers/driver_nl80211.c
cosnt struct wpa_driver_ops wpa_driver_nl80211_ops{
    .global_init = nl80211_global_init
}
    ->nl80211_global_init
        ->wpa_driver_nl80211_init_nl_global
```

## 整体流程总结

**对于上述流程我们可以总结为如下流程**

![hostapd处理netlink事件流程](./img/hostapd处理netlink事件流程.jpg)
