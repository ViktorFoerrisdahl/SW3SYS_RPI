# FILE: Makefile (WITH CAPITAL M!!)
obj-m := mygpio.o
ccflags-y := -std=gnu99 -Wno-declaration-after-statement

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

all default: modules
install: modules_install

modules modules_install help clean:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) $@

dtb:
	dtc -@ -I dts -O dtb -o mygpio-dt.dtbo mygpio-dt.dts
    