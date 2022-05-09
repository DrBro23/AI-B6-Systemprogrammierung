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

#define DRIVER_NAME "test_i2c"
#define DRIVER_CLASS "testClass"

static int __init dev_init(void);
static ssize_t dev_read(struct file *file, char *user_buffer, size_t count, loff_t *offs);
static ssize_t dev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset);
static void __exit dev_exit(void);

static dev_t myDeviceNr;
static struct class *myClass;
static struct cdev myDevice;

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = dev_read,
	.write = dev_write};

int dev_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	add_uevent_var(env, "DEVMODE=%#o", 0666);
	return 0;
}

int __init dev_init(void)
{
	printk(KERN_INFO "Fanctrl: starting %s. \n", __FUNCTION__);

	printk("Starting Temperature Sensor Subsystem");
	tempDeinit();
	tempInit();

	printk("Starting FOPS");
	alloc_chrdev_region(&myDeviceNr, 0, 1, DRIVER_NAME);
	myClass = class_create(THIS_MODULE, DRIVER_CLASS);
	myClass->dev_uevent = dev_uevent;
	device_create(myClass, NULL, myDeviceNr, NULL, DRIVER_NAME);
	cdev_init(&myDevice, &fops);
	cdev_add(&myDevice, myDeviceNr, 1);

	pwmInit();

	return 0;
}

unsigned int ToUInt(char* str)
{
    unsigned int mult = 1;
    unsigned int re = 0;
    int len = strlen(str);
	int i;
    for(i = len -1 ; i >= 0 ; i--)
    {
        re = re + ((int)str[i] - '0')*mult;
        mult = mult*10;
    }
    return re;
}

ssize_t dev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
	size_t max_len = 30, not_copied;
	u8 data_buf[max_len];
	u8 duty;

	if (count < max_len)
		max_len = count;
	not_copied = copy_from_user(data_buf, buf, max_len);
	if (not_copied == 0)
		printk(KERN_INFO "Copied %zd bytes from user\n", max_len);
	else
		printk(KERN_INFO "Could not copy %zd bytes from user\n", not_copied);
	data_buf[max_len - 1] = 0;
	printk(KERN_INFO "Device write: %s\n", data_buf);

	duty = ToUInt(data_buf);

	printk("data %d\n", duty);
	pwmSetDuty(duty);

	return count;
}

ssize_t dev_read(struct file *file, char *user_buffer, size_t count, loff_t *offs)
{
	// TODO
	// this functions does not exit on it's own
	// figure out copy buf len
	int to_copy, not_copied;
	u16 temp;
	size_t len;
	char out[20];

	temp = readTemp();

	// this sould be a floating point division,
	// however the compiler seems to have certain problems with the FPU
	len = snprintf(out, sizeof(out), "%d\n", temp / 1000);
	to_copy = min(len, count);
	not_copied = copy_to_user(user_buffer, out, to_copy);

	return (to_copy - not_copied);
}

void __exit dev_exit(void)
{
	printk(KERN_INFO "Fanctrl: stopping %s.\n", __FUNCTION__);

	tempDeinit();
	pwmDeinit();

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
