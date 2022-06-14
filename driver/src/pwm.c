// ----------------------------------------
// 
// ----------------------------------------

#include "pwm.h"

// Module parameters
static uint spi_bus = 0;
static uint spi_cs = 0;
static uint spi_speed_hz = 4000000;
static uint spi_bits_per_word = 8;

// Device creation for interaction with external device
static struct spi_device *spi_device;

// Initial function for PWM ( Pulse Width Modulation )
int pwmInit(void)  {
    struct spi_board_info spi_device_info = {
        .modalias = "module name",
        .max_speed_hz = spi_speed_hz,
        .bus_num = spi_bus,
        .chip_select = spi_cs,
        .mode = 0,
    };

    struct spi_master *master;
    int ret;

    printk(KERN_INFO "Fanctrl: Starting SPI");

    // Get the master device, given SPI the bus number
    master = spi_busnum_to_master( spi_device_info.bus_num );
    if( !master )	{
        printk(KERN_CRIT "Fanctrl: Failed to find Master");
	return -ENODEV;
    }

    printk(KERN_DEBUG "Fanctrl: Found Master");

    // Create a new slave device, given the master and device info
    spi_device = spi_new_device( master, &spi_device_info );
    if( !spi_device )	{
     	printk(KERN_CRIT "Fanctrl: Failed to set new Device");
	return -ENODEV;
    }

    printk(KERN_DEBUG "Fanctrl: Set device");
    spi_device->bits_per_word = spi_bits_per_word;

    ret = spi_setup( spi_device );
    if( ret )	{
    	printk(KERN_CRIT "Fanctrl: Failed to register");
	spi_unregister_device( spi_device );
	spi_device = NULL;
    }

    printk(KERN_DEBUG "Fanctrl: SPI Setup done %d", ret);
	pwmSetDuty(0x55);

    return ret;
}

void pwmDeinit(void)    {
    if(spi_device)	{
	spi_unregister_device(spi_device);
    }
}

void pwmSetDuty(int cycle)   {
  uint8_t tx[2];

  if( spi_device )
  {
    tx[0] = 0b00010011; // command byte
    tx[1] = cycle;
    spi_write(spi_device, &tx, 2);
  }
}
