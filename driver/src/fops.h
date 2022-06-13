#pragma once
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/kobject.h>

#define DRIVER_NAME "fanctrl"
#define DRIVER_CLASS "testClass"

int dev_uevent(struct device *dev, struct kobj_uevent_env *env);
ssize_t dev_read(struct file *file, char *user_buffer, size_t count, loff_t *offs);
ssize_t dev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset);
