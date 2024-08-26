# ubus编译

## 编译环境

编译器默认使用`gcc`
`cmake version>=3.13`



## 依赖关系

`ubus`编译依赖`json-c`和`libubox`

`libubox`依赖`json-c`



### 源码下载路径

```
git clone https://github.com/json-c/json-c.git
git clone http://git.openwrt.org/project/libubox.git或https://github.com/openwrt/libubox
git clone https://github.com/openwrt/ubus或https://github.com/openwrt/ubus
```



## 编译方法

下载`json-c、libubox、ubus`源代码，解压重并命名文件夹(不要修改源码)，保证文件目录结构如下。

复制粘贴运行本文档中的编译脚本，脚本运行参数`all`

```
.
├── build.sh
├── json-c
├── libubox
└── ubus
```



## 编译脚本

适用软件版本
`json-c`版本：`json-c-0.17-20230812`
`libubox`版本：`2024-5-17 eb9bcb64185ac155c02cc1a604692c4b00368324`
`ubus`版本：`2023-12-18 65bb027054def3b94a977229fd6ad62ddd32345b`

```powershell
#!/bin/bash

base_path=$(pwd)

jsonC_path=$base_path/json-c
jsonC_build_path=$base_path/jsonC_build

libubox_path=$base_path/libubox
libubox_build_path=$base_path/libubox_build

ubus_path=$base_path/ubus
ubus_build_path=$base_path/ubus_build

depend_path=$base_path/products_and_depends
depend_libs_path=$depend_path/depend_libs
depend_inc_path=$depend_path/depend_inc

# json-c库编译
jsonC_build() {
    echo "Start compiling jsonc"
    
    mkdir $jsonC_build_path
    cd $jsonC_build_path

    $jsonC_path/cmake-configure --prefix=./ --enable-shared

    make &&
    
    # 拷贝编译产物
    if [ ! -d "$depend_libs_path" ]; then
        mkdir -p $depend_libs_path
    fi

    cp $jsonC_build_path/libjson-c* $depend_libs_path
}

# libubox库编译
libubox_build() {
    echo "Start compiling libubox"

    mkdir $libubox_build_path
    cd $libubox_build_path

    # 拷贝依赖文件
    if [ ! -d "$depend_inc_path/json" ]; then
        mkdir -p $depend_inc_path/json
    fi

    cp $jsonC_build_path/json.h $depend_inc_path/json &&
    cp $jsonC_build_path/*.h $depend_inc_path &&
    cp $jsonC_path/*.h $depend_inc_path &&

    cmake -D json=$depend_libs_path/libjson-c.so -D CMAKE_C_FLAGS="-I $depend_inc_path"\
        -D BUILD_LUA:BOOL=OFF -D BUILD_EXAMPLES:BOLL=OFF -D BUILD_STATIC=ON $libubox_path

    make &&

    # 拷贝编译产物
    if [ ! -d "$depend_libs_path" ]; then
        mkdir -p $depend_libs_path
    fi

    cp $libubox_build_path/libblobmsg_json* $depend_libs_path &&
    cp $libubox_build_path/libjson_script* $depend_libs_path &&
    cp $libubox_build_path/libubox* $depend_libs_path
}

# ubus库编译
ubus_build() {
    echo "Start compiling ubus"

    mkdir $ubus_build_path
    cd $ubus_build_path

    # 拷贝依赖文件
    if [ ! -d "$depend_inc_path/libubox" ]; then
        mkdir -p $depend_inc_path/libubox
    fi
    cp $libubox_path/*.h $depend_inc_path/libubox &&

    cmake -D json=$depend_libs_path/libjson-c.so -D ubox_library=$depend_libs_path/libubox.so \
        -D blob_library=$depend_libs_path/libblobmsg_json.so -D ubox_include_dir:PATH=$depend_inc_path -D BUILD_LUA=OFF\
        -D BUILD_STATIC=ON $ubus_path

    make &&

    # 拷贝编译产物
    if [ ! -d "$depend_libs_path" ]; then
        mkdir -p $depend_libs_path
    fi

    cp $ubus_build_path/ubus $depend_path &&
    cp $ubus_build_path/ubusd $depend_path &&
    cp $ubus_build_path/libubus.* $depend_path &&
    cp $ubus_build_path/libubusd_library.* $depend_path
}

clean() {
    echo "Clean up old files"
    rm -rf $jsonC_build_path &&
    rm -rf $libubox_build_path &&
    rm -rf $ubus_build_path &&
    rm -rf $depend_path
}

build_all() {
    jsonC_build
    libubox_build
    ubus_build
}

# 检查传递的参数并调用相应的函数
case "$1" in
    jsonC)
        jsonC_build
        ;;
    libubox)
        libubox_build
        ;;
    ubus)
        ubus_build
        ;;
    all)
        build_all
        ;;
    clean)
        clean
        ;;
    *)
        echo "Usage: $0 [jsonC|libubox|ubus|all|clean]"
        exit 1
        ;;
esac
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
