#include <linux/ioctl.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

#include "temp.h"
#include "pwm.h"

#define CMD_READ_TEMP _IOR('a', 'a', int32_t*)
#define CMD_SET_SPEED _IOW('a', 'b', int32_t*)

long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
