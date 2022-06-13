#!/usr/bin/env python3

import ctypes
import array
from fcntl import ioctl
from ioctl_opt import IOR, IOW

filename = "/dev/fanctrl"

# #define CMD_READ_TEMP _IOR('a', 'a', int32_t*)
CMD_READ_TEMP = IOR(ord('A'), 0x01, ctypes.c_uint32)

# #define CMD_SET_SPEED _IOW('a', 'b', int32_t*)
CMD_SET_SPEED =IOW(ord('A'), 0x02, ctypes.c_uint32)

def set_speed(speed):
    buf = int.to_bytes(speed, 4, "little")
    fd = open(filename, "wb")
    rt = ioctl(fd, CMD_SET_SPEED, buf, False)
    if(rt != buf):
        print("Error Setting Speed")
    fd.close()

def read_temp():
    buf = bytes(4)
    fd = open(filename, "wb")
    rt = ioctl(fd, CMD_READ_TEMP, buf, False)
    temp = int.from_bytes(rt, "little")
    print(temp)
    fd.close()
    return temp


set_speed(255)
read_temp()
