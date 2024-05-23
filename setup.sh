#!/bin/bash

# you should make .pc file for dependency libraries to make build machine's pkg config properly find libs  

rm -r builddir
meson setup --cross-file arm.ini builddir
