#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>


#define CMD_1 _IOR('a', 'a', int32_t*)
#define CMD_2 _IOR('a', 'b', int32_t*)
#define CMD_3 _IOR('a', 'c', int32_t*)

int main(void)
{
	int fd = open("/dev/test_i2c", O_RDWR);
	
	if(fd < 0)
	{
		printf("Cannot open device file\n");
		return 0;
	}
	
	int32_t value = -1;
	char temp[20] = {};

	for (int i = 0; i <= 10; i++)
	{
		read(fd, temp, 20);
		printf("Value: %s", temp);
		memset(temp, 0, 20);
		sleep(1);
	}
	
	close(fd);
	
	return 0;
}
