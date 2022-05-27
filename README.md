# AI-B6-Systemprogrammierung

## Driver

### Install

```
cd driver
make all
sudo dtoverlay -d . spidev_disabler
sudo insmod fanctrl.ko
```

### SPI Hardware

|Signal| GPIO | Pin |
|--- | --- | --- |
| MOSI | GPIO10 | 19 |
| SCLK | GPIO11 | 23 |
| CE   |  GPIO8 | 24 |
