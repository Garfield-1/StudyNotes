# ubus编译

## 编译环境

编译器默认使用`gcc`
`cmake version>=3.13`



## 依赖关系

`ubus`编译依赖`json-c`和`libubox`

`libubox`依赖`json-c`



## 源码下载路径

```
git clone https://github.com/json-c/json-c.git
git clone https://github.com/openwrt/libubox
git clone https://github.com/openwrt/ubus
```



## 编译方法

下载`json-c、libubox、ubus`源代码，解压重并命名文件夹(不要修改源码)，保证文件目录结构如下。**同时保证`cmake`文件具有可执行权限**

复制粘贴运行本文档中的编译脚本，脚本运行参数`all`

```
.
├── build.sh
├── json-c
├── libubox
└── ubus
```



## 编译脚本

笔者测试用软件版本
`json-c`版本：`json-c-0.17-20230812`
`libubox`版本：`2024-5-17 eb9bcb64185ac155c02cc1a604692c4b00368324`
`ubus`版本：`2023-12-18 65bb027054def3b94a977229fd6ad62ddd32345b`

**编译产物**

* 编译完成后会创建一个`products_and_depends`名称的文件夹，用于存放编译产物`ubus`、`ubusd`和后续可能使用依赖库和头文件
* `ubus`源码目录下的`examples`也会一起被编译，这个是一个官方`demo`

```shell
#!/bin/bash

base_path=$(pwd)

jsonC_path=$base_path/json-c
jsonC_build_path=$base_path/jsonC_build

libubox_path=$base_path/libubox
libubox_build_path=$base_path/libubox_build

ubus_path=$base_path/ubus
ubus_build_path=$base_path/ubus_build
ubus_examples_build_path=$ubus_path/examples/build

depend_path=$base_path/products_and_depends
depend_libs_path=$depend_path/lib
depend_inc_path=$depend_path/include

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

    cmake -Djson=$depend_libs_path/libjson-c.so -DCMAKE_C_FLAGS="-I $depend_inc_path"\
        -DBUILD_LUA:BOOL=OFF -DBUILD_EXAMPLES:BOLL=OFF -DBUILD_STATIC=ON $libubox_path -DCMAKE_BUILD_TYPE=Debug

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

    cmake -Djson=$depend_libs_path/libjson-c.so -Dubox_library=$depend_libs_path/libubox.so \
        -Dblob_library=$depend_libs_path/libblobmsg_json.so -Dubox_include_dir:PATH=$depend_inc_path -DBUILD_LUA=OFF\
        -DBUILD_STATIC=ON $ubus_path -DCMAKE_BUILD_TYPE=Debug

    make &&

    # 拷贝编译产物
    if [ ! -d "$depend_libs_path" ]; then
        mkdir -p $depend_libs_path
    fi

    cp $ubus_path/*.h $depend_inc_path &&
    cp $ubus_build_path/ubus $depend_path &&
    cp $ubus_build_path/ubusd $depend_path &&
    cp $ubus_build_path/libubus.* $depend_libs_path &&
    cp $ubus_build_path/libubusd_library.* $depend_libs_path

    ubus_examples_build
}

# ubus官方demo编译
ubus_examples_build() {
    if [ ! -d "$ubus_examples_build_path" ]; then
        mkdir -p $ubus_examples_build_path
    fi

    cd $ubus_examples_build_path
    cmake .. -DBUILD_EXAMPLES=ON -DCMAKE_PREFIX_PATH=$depend_path -Dubox_library=ubox -DCMAKE_BUILD_TYPE=Debug
    make
}

clean() {
    echo "Clean up old files"
    rm -rf $jsonC_build_path &&
    rm -rf $libubox_build_path &&
    rm -rf $ubus_build_path &&
    rm -rf $depend_path &&
    rm -rf $ubus_examples_build_path
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

### 在终端手动执行ubus

创建三个终端

* **终端1：**

  ```shell
  su root
  
  ubusd	#ubusd进程
  ```

* **终端2：**

  ```shell
  su root
  
  ubus listen test	#监听ubus
  ```

* **终端3：**

  ```shell
  su root
  
  ubus send test '{"hi!":"hello!"}'	#发送ubus
  ```

如上述操作失败，可尝试将`json-c`、`libubox`和`ubus`的动态库库路径添加至系统动态库搜索路径

1. 切换`root`执行 ：`su root` 

2. 将动态库路径添加至系统动态库搜索路径：`echo [依赖库路径] >> /etc/ld.so.conf`

3. 更新动态库搜索路径：`ldconfig`


### 运行官方demo

在`ubus`源码中存在一个`examples`文件夹存，里面是一个官方`demo`，上文中的编译脚本也会编译这个`demo`

**运行方式**

创建三个终端

* **终端1：**

    ```shell
    su root
    
    ./ubusd			   #执行在编译产物（products_and_depends文件夹）中的ubusd
    ```

* **终端2：**

    ```shell
    su root
    
    ./server			#启动ubus/examples文件夹中的server端
    ```

    在`clinet`端也成功执行后`server`侧的终端会出现如下打印

    ```shell
    Watching object dd66ce76: Success
    Object dd66ce76 went away
    ```

* **终端3：**

    ```shell
    su root
    
    ./client			#启动ubus/examples文件夹中的client端
    ```

    在启动`clinet`端后终端会出现如下打印，并不断新增直到`client`端进程结束

    ```shell
    Avg time per iteration: 111 usec
    Subscribers active: 1
    Got fd from the server, watching...
    completed request, ret: 0
    Avg time per iteration: 111 usec
    Got line: msg1: test received a message: blah
    Sending count up to '100100'; string has length '592926'
    Server validated our count up to '100100'
    Got line: msg2: test received a message: blah
    Avg time per iteration: 116 usec
    Got line: msg3: test received a message: blah
    Avg time per iteration: 121 usec
    Got line: msg4: test received a message: blah
    Sending count up to '100200'; string has length '593629'
    Server validated our count up to '100200'
    Avg time per iteration: 106 usec
    Got line: msg5: test received a message: blah
    Avg time per iteration: 129 usec
    Got line: msg6: test received a message: blah
    Sending count up to '100300'; string has length '594333'
    ```

**如果运行时提示找不到依赖库则需手动配置环境依赖（设置 `LD_LIBRARY_PATH`）**

例如 `libubox.so` 位于 `/usr/local/lib` 目录下，执行以下命令；

**PS：**如果使用上文中的编译脚本执行`ubus`编译则`libubox.so`会在编译时存放在`products_and_depends/lib`文件中

```shell
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```

​	
