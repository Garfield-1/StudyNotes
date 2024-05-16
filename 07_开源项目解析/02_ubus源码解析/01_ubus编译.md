# ubus编译

## 编译环境

`cmake version>=3.13`



## 依赖关系

`ubus`编译依赖`json-c`和`libubox`



### 源码下载路径

```
git clone https://github.com/json-c/json-c.git
git clone http://git.openwrt.org/project/libubox.git
git clone https://git.openwrt.org/project/ubus.git或https://github.com/openwrt/ubus
```



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

