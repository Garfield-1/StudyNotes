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

