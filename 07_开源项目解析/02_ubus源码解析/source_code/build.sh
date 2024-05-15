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

    #拷贝依赖文件和编译产物
    if [ ! -d "$depend_inc_path/json" ]; then
        mkdir -p $depend_inc_path/json
    fi

    cp $jsonC_build_path/json.h $depend_inc_path/json &&
    cp $jsonC_build_path/*.h $depend_inc_path &&
    cp $jsonC_path/*.h $depend_inc_path &&
    
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

    cmake -D json=$depend_libs_path/libjson-c.so -D CMAKE_C_FLAGS="-I $depend_inc_path"\
        -D BUILD_LUA:BOOL=OFF -D BUILD_EXAMPLES:BOLL=OFF -D BUILD_STATIC=ON $libubox_path

    make &&

    #拷贝编译产物
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

    cmake -D json=$depend_libs_path/libjson-c.so -D ubox_library=$depend_libs_path/libubox.so \
        -D blob_library=$depend_libs_path/libblobmsg_json.so -D ubox_include_dir:PATH=$depend_inc_path -D BUILD_LUA=OFF\
        -D BUILD_STATIC=ON $ubus_path

    #拷贝依赖文件
    if [ ! -d "$depend_inc_path/libubox" ]; then
        mkdir -p $depend_inc_path/libubox
    fi
    cp $libubox_path/*.h $depend_inc_path/libubox &&

    make &&

    #拷贝编译产物
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
