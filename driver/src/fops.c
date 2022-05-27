#include "fops.h"
#include "pwm.h"
#include "temp.h"

dev_t myDeviceNr;
struct class *myClass;
struct cdev myDevice;

struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = dev_read,
	.write = dev_write
};


static int dev_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	add_uevent_var(env, "DEVMODE=%#o", 0666);
	return 0;
}

static unsigned int ToUInt(char* str)
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

void fops_init(void)    {
	printk(KERN_DEBUG "Fanctrl: Starting FOPS");

	alloc_chrdev_region(&myDeviceNr, 0, 1, DRIVER_NAME);
	myClass = class_create(THIS_MODULE, DRIVER_CLASS);
	myClass->dev_uevent = dev_uevent;
	device_create(myClass, NULL, myDeviceNr, NULL, DRIVER_NAME);
	cdev_init(&myDevice, &fops);
	cdev_add(&myDevice, myDeviceNr, 1);
}

void fops_deinit(void)  {
    printk(KERN_DEBUG "Fanctrl: Stopping FOPS");

    cdev_del(&myDevice);
    device_destroy(myClass, myDeviceNr);
    class_destroy(myClass);
    unregister_chrdev_region(myDeviceNr, 1);
}

ssize_t dev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
	size_t max_len = 30;
	size_t not_copied;
	uint8_t data_buf[30];
	uint8_t duty;

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
