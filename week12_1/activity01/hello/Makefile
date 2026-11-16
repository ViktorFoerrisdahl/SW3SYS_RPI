# FILE: Makefile (WITH CAPITAL M!!)
obj-m := hello.o
ccflags-y := -std=gnu99 -Wno-declaration-after-statement

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

all default: modules
install: modules_install

modules modules_install help clean:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) $@