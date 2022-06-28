// ----------------------------------------
// Driver Main
// ----------------------------------------

#define PROJECT_LICENSE	"GPL"
#define PROJECT_AUTHOR_NAME	"Sven Menzel & Robin Prillwitz"
#define PROJECT_DESCRIPTION	"Fanctrl Module"
#define PROJECT_VERSION "1.0.0"

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/kernel.h>

#include "temp.h"
#include "pwm.h"
#include "fops.h"
#include "ioctl.h"

// Initialization
static int __init dev_init(void);
static void __exit dev_exit(void);

// kernel device
dev_t deviceNr;
struct class *deviceClass;
struct cdev cdevDevice;

// File Operations for fan
struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = dev_read,
	.write = dev_write,
	.unlocked_ioctl = dev_ioctl,
};

// main entry
int __init dev_init(void)
{
	printk(KERN_INFO "Fanctrl: Starting\n");

	printk(KERN_DEBUG "Fanctrl: Starting I2C Temperature Sensor\n");
	tempDeinit();
	tempInit();

	printk(KERN_DEBUG "Fanctrl: Starting SPI\n");
	pwmDeinit();
	pwmInit();

	printk(KERN_DEBUG "Fanctrl: Registering Kernel Device\n");

	alloc_chrdev_region(&deviceNr, 0, 1, DRIVER_NAME);
	deviceClass = class_create(THIS_MODULE, DRIVER_CLASS);
	deviceClass->dev_uevent = dev_uevent;

	device_create(deviceClass, NULL, deviceNr, NULL, DRIVER_NAME);
	cdev_init(&cdevDevice, &fops);
	cdev_add(&cdevDevice, deviceNr, 1);

	printk(KERN_INFO "Fanctrl: Initlization Done\n");

	return 0;
}

// main exit
void __exit dev_exit(void)
{
	printk(KERN_INFO "Fanctrl: Stopping\n");

	printk(KERN_DEBUG "Fanctrl: Stopping I2C\n");
	tempDeinit();

	printk(KERN_DEBUG "Fanctrl: Stopping SPI\n");
	pwmDeinit();

	printk(KERN_DEBUG "Fanctrl: Deinitilizing Kernel device\n");
    cdev_del(&cdevDevice);
    device_destroy(deviceClass, deviceNr);
    class_destroy(deviceClass);
    unregister_chrdev_region(deviceNr, 1);

	printk(KERN_INFO "Fanctrl: Deinitilizing done\n");
}

module_init(dev_init);
module_exit(dev_exit);

MODULE_LICENSE(PROJECT_LICENSE);
MODULE_AUTHOR(PROJECT_AUTHOR_NAME);
MODULE_DESCRIPTION(PROJECT_DESCRIPTION);
MODULE_VERSION(PROJECT_VERSION);
