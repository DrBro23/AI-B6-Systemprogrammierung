obj-m += fanctrl.o
fanctrl-objs := ./src/fanctrl.o ./src/temp.o ./src/pwm.o

KDIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
