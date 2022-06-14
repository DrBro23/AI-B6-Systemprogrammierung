#!/usr/bin/env python3

import ctypes
import csv
import math
from fcntl import ioctl
from time import sleep
from ioctl_opt import IOR, IOW

curve = "./curve.csv"
driver = "/dev/fanctrl"
curve_points = list()
MIN_SPEED = 255
MAX_SPEED = 0

# #define CMD_SET_SPEED _IOW('A', 0x02, int32_t*)
CMD_SET_SPEED =IOW(ord('A'), 0x02, ctypes.c_uint32)

def set_speed(speed):
    buf = int.to_bytes(speed, 4, "little")
    fd = open(driver, "wb")
    rt = ioctl(fd, CMD_SET_SPEED, buf, False)
    if(rt != buf):
        print("Error Setting Speed")
    fd.close()

# #define CMD_READ_TEMP _IOR('A', 0x01, int32_t*)
CMD_READ_TEMP = IOR(ord('A'), 0x01, ctypes.c_uint32)

def read_temp():
    buf = bytes(4)
    fd = open(driver, "wb")
    rt = ioctl(fd, CMD_READ_TEMP, buf, False)
    temp = int.from_bytes(rt, "little")
    fd.close()
    return temp

def parse_csv():
    with open(curve, mode ='r') as file:
        csvFile = csv.reader(file, delimiter=",")

        for index, lines in enumerate(csvFile):
            if index == 0:
                continue
            else:
                curve_points.append((int(lines[0]), int(lines[1])))

def interpolate_curve(x):
    min_value = 0
    min_index = 0
    max_value = 9999999999999
    max_index = 0

    for index, element in enumerate(curve_points):
        if((element[0] >= min_value) and (element[0] <= x)):
            min_value = element[0]
            min_index = index
        if((element[0] < max_value) and (element[0] > x)):
            max_value = element[0]
            max_index = index

    interp = curve_points[min_index][1] + (curve_points[max_index][1] - curve_points[min_index][1]) * ((x - min_value) / (max_value - min_value))
    interp_round = min(max(round(interp), 0), 100)
    return interp_round

def percentage_to_speed(x):
    return round( MIN_SPEED + ((MAX_SPEED - MIN_SPEED) / (100 - 0)) * (x - 0) )

if __name__ == "__main__":
    parse_csv()
    while 1:
        temp = read_temp()
        speed = interpolate_curve(temp)
        speed_pwm = percentage_to_speed(speed)
        set_speed(speed_pwm)

        print(f"{temp}°C -> {speed}% ({speed_pwm})")
        sleep(1)
