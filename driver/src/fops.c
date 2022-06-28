// ----------------------------------------
// Input and Output for user
// ----------------------------------------

#include "fops.h"
#include "pwm.h"
#include "temp.h"

// Add user event key-value pair to enviornment Buffer
int dev_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	add_uevent_var(env, "DEVMODE=%#o", 0666);
	return 0;
}

// Convert string to int
static unsigned int ToUInt(char *str)
{
	unsigned int mult = 1;
	unsigned int re = 0;
	int len = strlen(str);
	int i;
	for (i = len - 1; i >= 0; i--)
	{
		re = re + ((int)str[i] - '0') * mult;
		mult = mult * 10;
	}
	return re;
}

// --- fops write start ---
ssize_t dev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
	size_t max_len = 30;
	size_t not_copied;
	u8 data_buf[30];
	u8 duty;

	if (count < max_len)
	{
		max_len = count;
	}

	not_copied = copy_from_user(data_buf, buf, max_len);
	if (not_copied == 0)
	{
		printk(KERN_DEBUG "Fanctrl: Copied %zd bytes from user\n", max_len);
	}
	else
	{
		printk(KERN_WARNING "Fanctrl: Could not copy %zd bytes from user\n", not_copied);
	}
	data_buf[max_len - 1] = 0;
	printk(KERN_DEBUG "Fanctrl: Device write: %s\n", data_buf);

	duty = ToUInt(data_buf);

	printk("data %d\n", duty);
	pwmSetDuty(duty);

	return count;
}
// --- fops write end ---

// --- fops read start ---
ssize_t dev_read(struct file *file, char *user_buffer, size_t count, loff_t *offs)
{
	int to_copy, not_copied;
	u16 temp;
	size_t len;
	char out[20];

	temp = readTemp();

	len = snprintf(out, sizeof(out), "%d°C\r\n", temp);
	to_copy = min(len, count);
	not_copied = copy_to_user(user_buffer, out, to_copy);

	return (to_copy - not_copied);
}
// --- fops read end ---
