#!/bin/bash
@echo off

cd driver
echo "Building driver..."
make clean
make all
echo "Unloading active driver"
sudo rmmod fanctrl
echo "Disabeling SPI0 CE Pin"
sudo dtoverlay -d . spidev_disabler
echo "Loading driver"
sudo insmod fanctrl.ko
echo "Done"
echo "View Kernel Messages using: dmesg -w"
