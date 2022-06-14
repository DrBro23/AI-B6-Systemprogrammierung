#include "pwm.h"

// spi device parameters
static uint spi_bus = 0;
static uint spi_cs = 0;
static uint spi_speed_hz = 4000000;
static uint spi_bits_per_word = 8;

static struct spi_device *spi_device;

// --- spi init start ---
int pwmInit(void)
{
    int ret;
    struct spi_master *master;
    struct spi_board_info spi_device_info = {
        .modalias = "module name",
        .max_speed_hz = spi_speed_hz,
        .bus_num = spi_bus,
        .chip_select = spi_cs,
        .mode = 0,
    };

    master = spi_busnum_to_master(spi_device_info.bus_num);
    if (!master)
    {
        printk(KERN_CRIT "Fanctrl: Failed to find Master");
        return -ENODEV;
    }

    spi_device = spi_new_device(master, &spi_device_info);
    if (!spi_device)
    {
        printk(KERN_CRIT "Fanctrl: Failed to set new Device");
        return -ENODEV;
    }

    spi_device->bits_per_word = spi_bits_per_word;
    ret = spi_setup(spi_device);
    if (ret)
    {
        printk(KERN_CRIT "Fanctrl: Failed to register");
        spi_unregister_device(spi_device);
        spi_device = NULL;
    }

    pwmSetDuty(0x55);
    return ret;
}
// --- spi init end ---

// --- spi deinit start ---
void pwmDeinit(void)
{
    if (spi_device)
    {
        spi_unregister_device(spi_device);
    }
}
// --- spi deinit end ---

// --- spi write start ---
void pwmSetDuty(int cycle)
{
    uint8_t tx[2];

    if (spi_device)
    {
        tx[0] = 0b00010011; // command byte
        tx[1] = cycle;
        spi_write(spi_device, &tx, 2);
    }
}
// --- spi write end ---
