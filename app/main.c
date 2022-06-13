#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define CMD_READ_TEMP _IOR('a', 'a', int32_t *)
#define CMD_SET_SPEED _IOW('a', 'b', int32_t *)

int main(void)
{
	int fd = open("/dev/fanctrl", O_RDWR);
	if (fd < 0)
	{
		printf("Cannot open device file\n");
		return 0;
	}
	int32_t value = -1;

	ioctl(fd, CMD_READ_TEMP, &value);
	printf("VALUE: %d\n", value);

	value = 127;
	ioctl(fd, CMD_SET_SPEED, &value);
	printf("VALUE: %d\n", value);

	close(fd);
	return 0;
}
