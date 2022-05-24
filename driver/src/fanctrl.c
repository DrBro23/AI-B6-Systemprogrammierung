#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/kernel.h>

#include "temp.h"
#include "pwm.h"
#include "fops.h"

static int __init dev_init(void);
static void __exit dev_exit(void);


int __init dev_init(void)
{
	printk(KERN_INFO "Fanctrl: starting %s. \n", __FUNCTION__);

	tempDeinit();
	tempInit();

	pwmDeinit();
	pwmInit();

	fops_init();

	printk(KERN_INFO "Fanctrl: Init Done");

	return 0;
}

void __exit dev_exit(void)
{
	printk(KERN_INFO "Fanctrl: stopping %s.\n", __FUNCTION__);

	tempDeinit();
	pwmDeinit();

	fops_deinit();
}

module_init(dev_init);
module_exit(dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sven Menzel & Robin Prillwitz");
MODULE_DESCRIPTION("Projekt DeviceDriver Modul");
MODULE_VERSION("1.0");
