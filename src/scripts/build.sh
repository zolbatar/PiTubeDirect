#!/bin/sh
make -Bj
cp kernel.img /Volumes/SDCARD
diskutil unmount /dev/disk2s1
