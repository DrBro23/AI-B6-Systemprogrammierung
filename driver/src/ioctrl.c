#include "ioctrl.h"

long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    uint32_t value = 0;

    switch (cmd)
    {
    case CMD_READ_TEMP:
    {
        value = readTemp();

        if (copy_to_user((uint32_t *)arg, &value, sizeof(value)))    {
            pr_err("Read: Error!\n");
        }
        printk("ioctrl temp value:%d (%d)\n", value, cmd);

        break;
    }
    case CMD_SET_SPEED:
    {
        if( copy_from_user(&value ,(uint32_t*) arg, sizeof(value)) ) {
            pr_err("Write: Error!\n");
        }
        pwmSetDuty(value);
        printk("ioctrl duty value:%d\n", value);

        break;
    }
    default:
        // pr_err("Invalid IOCTRL Command (%d)\n", cmd);
        break;
    }
    return 0;
}
