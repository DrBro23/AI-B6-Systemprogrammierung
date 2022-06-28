// ----------------------------------------
// Input/Output Control for temperature read and speed fan set
// ----------------------------------------

#include "ioctl.h"

// --- ioctl start ---
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
            printk(KERN_CRIT "Fanctrl: Write Error!\n");
        }
        printk(KERN_DEBUG "Fanctrl: IOCTL temp value: %d\n", value);

        break;
    }
    case CMD_SET_SPEED:
    {
        if (copy_from_user(&value, (u32 *)arg, sizeof(value)))
        {
            printk(KERN_CRIT "Fanctrl: Write Error!\n");
        }
        pwmSetDuty(value);
        printk(KERN_DEBUG "Fanctrl: IOCTL duty value: %d\n", value);

        break;
    }
    default:
        break;
    }
    return 0;
}
// --- ioctl end ---
