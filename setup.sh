#!/bin/bash

# you should make .pc file for dependency libraries to make build machine's pkg config properly find libs  
export PKG_CONFIG_PATH=/home/byunghun/workspace/luckfox-pico/sysdrv/source/buildroot/buildroot-2023.02.6/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/lib/pkgconfig/

rm -r builddir
meson setup --cross-file arm.ini builddir
