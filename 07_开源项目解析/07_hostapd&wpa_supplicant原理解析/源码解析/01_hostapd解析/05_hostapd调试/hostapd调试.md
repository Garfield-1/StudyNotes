# hostapd调试



## 调试环境

* 树莓派`4B`

    系统`Linux pi 5.15.84-v8+ #1613 SMP PREEMPT Thu Jan 5 12:03:08 GMT 2023 aarch64 GNU/Linux`

* 调试版本

    `hostapd-2019-08-08-ca8c2bd2`



## 编译方法

进入`hostapd-2019-08-08-ca8c2bd2/hostapd`目录执行

```shell
cp defconfig .config
make
```



## 运行调试

执行如下命令运行调试

```shell
sudo ./hostapd ./hostapd.conf
```

运行命令之前先关闭可能有冲突的程序

```shell
# 查看网络接口状态
ip link show wlan0

# 查看是否有其他进程占用接口
sudo lsof | grep wlan0

# 停止可能冲突的网络服务
sudo systemctl stop NetworkManager
sudo systemctl stop wpa_supplicant
sudo killall wpa_supplicant
```

