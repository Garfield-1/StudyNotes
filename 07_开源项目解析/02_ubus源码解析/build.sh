#!/bin/bash

base_path=$(pwd)

jsonC_path=$base_path/json-c
jsonC_build_path=$base_path/json

libubox_path=$base_path/libubox
libubox_build_path=$base_path/libubox_build

ubus_path=$base_path/ubus
ubus_build_path=$base_path/ubus_build

libs_path=$base_path/libs
depend_libs_path=$base_path/libs/depend_libs

# json-c库编译
jsonC_build() {
    echo "Start compiling jsonc"
    
    mkdir $jsonC_build_path
    cd $jsonC_build_path

    $jsonC_path/cmake-configure --prefix=./ --enable-shared

    make &
}

# libubox库编译
libubox_build() {
    echo "Start compiling libubox"

    mkdir $libubox_build_path
    cd $libubox_build_path

    cmake -D json=libjson-c.so -D CMAKE_C_FLAGS="-I $base_path -I $jsonC_build_path \
        -I $jsonC_path" -D BUILD_LUA:BOOL=OFF -D BUILD_EXAMPLES:BOLL=OFF\
        -D BUILD_STATIC=ON $libubox_path

    make &
}

# ubus库编译
ubus_build() {
    echo "Start compiling ubus"

    mkdir $ubus_build_path
    cd $ubus_build_path

    # cmake -D BUILD_STATIC=ON -D ubox_library=$depend_libs_path  $ubus_path
#  -D UBOX_LIBRARY_PATH=$libubox_build_path -D BLOB_LIBRARY_PATH=$libubox_build_path -D 
    cmake -D json=libjson-c.so -D ubox_library=libubox \
        -D blob_library=libblobmsg-json.so -D ubox_include_dir:PATH=$base_path -D BUILD_LUA=OFF\
        -D BUILD_STATIC=ON $ubus_path

    make &
}

clean() {
    echo "Clean up old files"
    rm -rf $jsonC_build_path
    rm -rf $libubox_build_path
}

copy_libs() {
    echo "copy_libs"
    rm -rf $libs_path
    mkdir -p $depend_libs_path
    cp $jsonC_build_path/libjson-c* $depend_libs_path
    cp $libubox_build_path/libubox* $depend_libs_path
}

build() {
    jsonC_build
    libubox_build
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
        build
        ;;
    copy)
        copy_libs
        ;;
    clean)
        clean
        ;;
    *)
        echo "Usage: $0 [jsonC|libubox|ubus|all|clean]"
        exit 1
        ;;
esac
