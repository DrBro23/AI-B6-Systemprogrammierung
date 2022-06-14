// ----------------------------------------
// Communication between driver and fan
// ----------------------------------------

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
#include "ioctrl.h"

// Initial functions
static int __init dev_init(void);
static void __exit dev_exit(void);

// Driver functions
dev_t myDeviceNr;
struct class *myClass;
struct cdev myDevice;

// File Operations for fan
struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = dev_read,
	.write = dev_write,
	.unlocked_ioctl = dev_ioctl,
};

int __init dev_init(void)
{
	printk(KERN_INFO "Fanctrl: starting %s. \n", __FUNCTION__);

	tempDeinit();
	tempInit();

	pwmDeinit();
	pwmInit();

	printk(KERN_DEBUG "Fanctrl: Starting FOPS");

	alloc_chrdev_region(&myDeviceNr, 0, 1, DRIVER_NAME);
	myClass = class_create(THIS_MODULE, DRIVER_CLASS);
	myClass->dev_uevent = dev_uevent;
	device_create(myClass, NULL, myDeviceNr, NULL, DRIVER_NAME);
	cdev_init(&myDevice, &fops);
	cdev_add(&myDevice, myDeviceNr, 1);

	printk(KERN_INFO "Fanctrl: Init Done");

	return 0;
}

void __exit dev_exit(void)
{
	printk(KERN_INFO "Fanctrl: Stopping %s.\n", __FUNCTION__);

	tempDeinit();
	pwmDeinit();

	printk(KERN_DEBUG "Fanctrl: Stopping FOPS");

    cdev_del(&myDevice);
    device_destroy(myClass, myDeviceNr);
    class_destroy(myClass);
    unregister_chrdev_region(myDeviceNr, 1);
}

module_init(dev_init);
module_exit(dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sven Menzel & Robin Prillwitz");
MODULE_DESCRIPTION("Projekt DeviceDriver Modul");
MODULE_VERSION("1.0");
