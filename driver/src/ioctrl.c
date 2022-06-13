#include "ioctrl.h"

long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int32_t value = 0;

    switch (cmd)
    {
    case CMD_READ_TEMP:
    {
        value = readTemp();

        if (copy_to_user((int32_t *)arg, &value, sizeof(value)))    {
            pr_err("Read: Error!\n");
        }

        break;
    }
    case CMD_SET_SPEED:
    {
        if( copy_from_user(&value ,(int32_t*) arg, sizeof(value)) ) {
            pr_err("Write: Error!\n");
        }
        pwmSetDuty(value);

        break;
    }
    default:
        pr_err("Invalid IOCTRL Command (%d)\n", cmd);
        break;
    }
    printk("ioctrl value:%d\n", value);
    return 0;
}
