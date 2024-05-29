#!/bin/bash

export PKG_CONFIG_PATH=/home/byunghun/workspace/luckfox-pico/sysdrv/source/buildroot/buildroot-2023.02.6/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/lib/pkgconfig

ninja -C builddir
