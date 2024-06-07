# ubus编译

## 编译环境

`cmake version>=3.13`



## 依赖关系

`ubus`编译依赖`json-c`和`libubox`



### 源码下载路径

```
git clone https://github.com/json-c/json-c.git
git clone http://git.openwrt.org/project/libubox.git或https://github.com/openwrt/libubox
git clone https://github.com/openwrt/ubus或https://github.com/openwrt/ubus
```

本文档使用软件版本

`json-c`版本：`json-c-0.17-20230812`

`libubox`版本：`2024-5-17 eb9bcb64185ac155c02cc1a604692c4b00368324`

`ubus`版本：`2023-12-18 65bb027054def3b94a977229fd6ad62ddd32345b`



## 编译方法

将项目中`source_code`目录中的压缩包解压，执行`build.sh`即可

需保证文件目录结构如下

```
.
├── build.sh
├── json-c
├── libubox
└── ubus
```



## 在Ubuntu上运行ubus

### 在Ubuntu上安装相关库

在完成上文中的编译步骤后，依次进入`jsonC_build`、`libubox_build`和`ubus_build`文件夹执行`sudo make install`命令在`Ubuntu`上安装`json-c`、`libubox`和`ubus`

> 笔者注：此处建议使用包管理工具先对编译成生产的二进制进行打包，然后再安装。这样可以使用包管理器进行管理

### 测试执行ubus

创建三个终端

* **终端1：**

  `su root` 切换`root`

​	`ubusd`  拉起`ubusd`进程

* **终端2：**

  `su root` 切换`root`

​	`ubus listen test` //监听`ubus`

* **终端3：**

  `su root` 切换`root`

​	`ubus send test '{"hi!":"hello!"}'`



如上述操作失败，可尝试将`json-c`、`libubox`和`ubus`的动态库库路径添加至系统动态库搜索路径

1. 切换`root`执行 ：`su root` 

2. 将动态库路径添加至系统动态库搜索路径：`echo [依赖库路径] >> /etc/ld.so.conf`

3. 更新动态库搜索路径：`ldconfig`
