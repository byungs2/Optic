#!/bin/bash

gcc -g -o test test.c `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0`
