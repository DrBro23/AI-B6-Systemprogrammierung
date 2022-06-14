// ----------------------------------------
// Input/Output Control for temperature read and speed fan set
// ----------------------------------------

#include "ioctrl.h"

long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    u32 value = 0;

    switch (cmd)
    {
    case CMD_READ_TEMP:
    {
        value = readTemp();

        if (copy_to_user((u32 *)arg, &value, sizeof(value)))
        {
            pr_err(KERN_INFO "Read: Error!\n");
        }
        printk(KERN_INFO "IOCTRL temp value: %d (%d)\n", value, cmd);

        break;
    }
    case CMD_SET_SPEED:
    {
        if (copy_from_user(&value, (u32 *)arg, sizeof(value)))
        {
            pr_err(KERN_INFO "Write: Error!\n");
        }
        pwmSetDuty(value);
        printk(KERN_INFO "IOCTRL duty value: %d\n", value);

        break;
    }
    default:
        // pr_err("Invalid IOCTRL Command (%d)\n", cmd);
        break;
    }
    return 0;
}
