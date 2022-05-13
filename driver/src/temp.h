#pragma once

#include <linux/module.h>
#include <linux/init.h>

#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/kernel.h>

#define I2C_BUS_AVAILABLE 1
#define SLAVE_DEVICE_NAME "deine mutter...nicht in diesem ausdruck!"
#define SLAVE_ADDRESS 0x48

void tempInit(void);
void tempDeinit(void);

u16 readTemp(void);
