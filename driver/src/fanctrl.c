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

// Temperatursensor
#define DRIVER_NAME "test_i2c"
#define DRIVER_CLASS "testClass"
#define I2C_BUS_AVAILABLE 1
#define SLAVE_DEVICE_NAME "tmp102"
#define SLAVE_ADDRESS 0x48

// Motor
#define PWM 	20
#define RPM		21

static int __init dev_init(void);
static ssize_t dev_read(struct file *file, char *user_buffer, size_t count, loff_t *offs);
static ssize_t dev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset);
static void __exit dev_exit(void);

static struct i2c_adapter *test_i2c_adapter = NULL;
static struct i2c_client *test_i2c_client = NULL;
static const struct i2c_device_id test_id[] =
	{
		{SLAVE_DEVICE_NAME, 0},
		{}};

static struct i2c_driver test_driver = {
	.driver = {.name = SLAVE_DEVICE_NAME, .owner = THIS_MODULE}};
static struct i2c_board_info test_i2c_board_info = {
	I2C_BOARD_INFO(SLAVE_DEVICE_NAME, SLAVE_ADDRESS)};

static dev_t myDeviceNr;
static struct class *myClass;
static struct cdev myDevice;

static struct cdev cdev;
static int dev_major = 0;
static struct class *dev_class = NULL;

int dev_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	add_uevent_var(env, "DEVMODE=%#o", 0666);
	return 0;
}

static struct file_operations fopsTemp = {
	.owner = THIS_MODULE,
	.read = dev_read
};

static struct file_operations fopsMotor = {
	.owner = THIS_MODULE,
	.write = dev_write,
	.read = dev_read
};

int __init dev_init(void)
{

	printk("TEST 1");

	my_test();

	printk("TEST 2");


	// int ret = -1;
	// // Motor
	// gpio_is_valid(PWM);			// PWM write
	// gpio_is_valid(RPM);			// RPM read

	// gpio_request(PWM, "PWM");		// output wegen write
	// gpio_request(RPM, "RPM");		// input wegen read

	// gpio_direction_output(PWM, 0);
	// gpio_direction_input(RPM);

	// gpio_export(PWM, false);
	// gpio_export(RPM, false);


	// ret = -2;
	// // Temperatursensor via I2C
	// alloc_chrdev_region(&myDeviceNr, 0, 1, DRIVER_NAME);
	// myClass = class_create(THIS_MODULE, DRIVER_CLASS);
	// myClass->dev_uevent = dev_uevent;
	// device_create(myClass, NULL, myDeviceNr, NULL, DRIVER_NAME);

	// cdev_init(&myDevice, &fopsTemp);
	// cdev_add(&myDevice, myDeviceNr, 1);

	// test_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);
	// test_i2c_client = i2c_new_client_device(test_i2c_adapter,
	// 										&test_i2c_board_info);
	// i2c_add_driver(&test_driver);
	// ret = 0;
	// i2c_put_adapter(test_i2c_adapter);
	// return ret;
	return 0;
}

ssize_t dev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
	uint8_t data;

	while(1)
	{
		int ret = copy_from_user(&data, buf, 1);
		if (ret >= 0 || ret );
	}

	return count;

}

ssize_t dev_read(struct file *file, char *user_buffer, size_t count, loff_t *offs)
{
	int to_copy, not_copied;
	int temp;
	size_t len;
	char out[20];
	u8 b1, b2;

	i2c_smbus_write_byte(test_i2c_client, 0x00);
	b1 = i2c_smbus_read_byte(test_i2c_client);
	b2 = i2c_smbus_read_byte(test_i2c_client);

	temp = (u16)b1 << 4 | b2 >> 4;
	temp = temp * 625;
	len = snprintf(out, sizeof(out), "%d\n", temp / 10000);
	to_copy = min(len, count);
	not_copied = copy_to_user(user_buffer, out, to_copy);
	printk("READ: %d\n", to_copy);
	return (to_copy - not_copied);
}


void __exit dev_exit(void)
{
	// // Motor
	// gpio_unexport(PWM);
	// gpio_unexport(RPM);

	// // Temperatursensor
	// i2c_unregister_device(test_i2c_client);
	// i2c_del_driver(&test_driver);
	// cdev_del(&myDevice);
	// device_destroy(myClass, myDeviceNr);
	// class_destroy(myClass);
	// unregister_chrdev_region(myDeviceNr, 1);
}

module_init(dev_init);
module_exit(dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sven Menzel & Robin Prillwitz");
MODULE_DESCRIPTION("Projekt DeviceDriver Modul");
MODULE_VERSION("1.0");
