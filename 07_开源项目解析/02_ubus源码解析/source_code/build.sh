#!/bin/bash

base_path=$(pwd)

jsonC_path=$(pwd)/json-c
jsonC_build_path=$(pwd)/jsonC_build

libubox_path=$(pwd)/libubox
libubox_build_path=$(pwd)/libubox_build

ubus_path=$(pwd)/ubus
ubus_build_path=$(pwd)/ubus_build

# json-c库编译
function jsonC_build {
    echo "Start compiling jsonc"
    
    mkdir $jsonC_build_path
    cd $jsonC_build_path

    $jsonC_path/cmake-configure --prefix=$jsonC_build_path --enable-shared

    make
}

# libubox库编译
function libubox {
    echo "Start compiling libubox"
}

# ubus库编译
function ubus {
    echo "Start compiling ubus"
}

function clean {
    echo "Clean up old files"
    rm -rf $jsonC_build_path
}

clean
jsonC_build