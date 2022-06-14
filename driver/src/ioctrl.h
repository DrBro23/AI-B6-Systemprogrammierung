// ----------------------------------------
// IOCTRL Header file
// ----------------------------------------

#include <linux/ioctl.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

#include "temp.h"
#include "pwm.h"

#define CMD_READ_TEMP _IOR('A', 0x01, u32*)
#define CMD_SET_SPEED _IOW('A', 0x02, u32*)

long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
