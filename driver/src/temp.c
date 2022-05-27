#include "temp.h"

// Communication with the temperature sensor
// by I2C
static struct i2c_adapter *test_i2c_adapter = NULL;
static struct i2c_client *test_i2c_client = NULL;
static const struct i2c_device_id test_id[] =
{
        {SLAVE_DEVICE_NAME, 0},
        {}
};
static struct i2c_driver test_driver = {
    .driver = {.name = SLAVE_DEVICE_NAME, .owner = THIS_MODULE}};
static struct i2c_board_info test_i2c_board_info = {
    I2C_BOARD_INFO(SLAVE_DEVICE_NAME, SLAVE_ADDRESS)};

void tempInit(void) {

    test_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);
	if(test_i2c_adapter == NULL)	{
		printk(KERN_CRIT "Fanctrl: No I2C Adapter found");
	}	else	{
		printk(KERN_DEBUG "Fanctrl: Found I2C Adapter");
		test_i2c_client = i2c_new_client_device(test_i2c_adapter, &test_i2c_board_info);
    	i2c_add_driver(&test_driver);
    	i2c_put_adapter(test_i2c_adapter);
	}
}

void tempDeinit(void)   {
	if(test_i2c_client != NULL)	{
    	i2c_unregister_device(test_i2c_client);
    	i2c_del_driver(&test_driver);
	}
}

// read temperatur value from the temperature sensor
u16 readTemp(void) {
	int temp;
	u8 b1, b2;

	i2c_smbus_write_byte(test_i2c_client, 0x00);
	b1 = i2c_smbus_read_byte(test_i2c_client);
	b2 = i2c_smbus_read_byte(test_i2c_client);

	temp = (u16)b1 << 4 | b2 >> 4;
	temp = temp * 625;

	return temp;
}
