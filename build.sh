#!/bin/bash

SYSROOT=/home/byunghun/workspace/luckfox-pico/sysdrv/source/buildroot/buildroot-2023.02.6/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot
echo $SYSROOT
export LD_LIBRARY_PATH=$SYSROOT/usr/lib
export C_INCLUDE_PATH=$SYSROOT/usr/include
export CPLUS_INCLUDE_PATH=$SYSROOT/usr/include

echo "SETUP ENV"
echo $LD_LIBRARY_PATH
echo $CPLUS_INCLUDE_PATH

/home/byunghun/workspace/luckfox-pico/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf-g++ main.c -o test -I $SYSROOT/usr/include -I$SYSROOT/usr/include/opencv4 -L$SYSROOT/usr/lib -lopencv_core -lz
#gcc main.c -o test `pkg-config --cflags gstreamer-1.0` `pkg-config --libs gstreamer-1.0`
