#pragma once

#include <linux/init.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/spi/spi.h>

int pwmInit(void);
void pwmDeinit(void);

void pwmSetDuty(int cycle);
