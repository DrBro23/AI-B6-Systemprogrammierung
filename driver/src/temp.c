// ----------------------------------------
// I2C Temperature Sensor
// ----------------------------------------

#include "temp.h"

// I2C Device and Adapter
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

// --- i2c init start ---
void tempInit(void) {
    test_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);
	if(test_i2c_adapter == NULL)	{
		printk(KERN_CRIT "Fanctrl: No I2C Adapter found\n");
	}	else	{
		printk(KERN_DEBUG "Fanctrl: Found I2C Adapter\n");
		test_i2c_client = i2c_new_client_device(test_i2c_adapter, &test_i2c_board_info);
    	i2c_add_driver(&test_driver);
    	i2c_put_adapter(test_i2c_adapter);
	}
}
// --- i2c init end ---

// --- i2c deinit start ---
void tempDeinit(void)   {
	if(test_i2c_client != NULL)	{
    	i2c_unregister_device(test_i2c_client);
    	i2c_del_driver(&test_driver);
	}
}
// --- i2c deinit end ---

// --- i2c read start ---
u16 readTemp(void) {
	int temp;
	u16 b1, b2;

	i2c_smbus_write_byte(test_i2c_client, 0x00);
	b1 = (u16)i2c_smbus_read_byte(test_i2c_client);
	b2 = (u16)i2c_smbus_read_byte(test_i2c_client);

	temp = (b1 << 4) | (b2 >> 4);
	temp = temp / 16;

	return temp;
}
// --- i2c read end ---
