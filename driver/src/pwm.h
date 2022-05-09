#pragma once

#include <linux/io.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/kthread.h>

#define PWM_PIN 17
#define THREAD_NAME "PWM"

void pwmInit(void);
void pwmDeinit(void);

void pwmSetDuty(int cycle);
