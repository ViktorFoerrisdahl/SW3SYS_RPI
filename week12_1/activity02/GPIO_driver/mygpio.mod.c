#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xe7ffb744, "gpiod_put" },
	{ 0x4c0a62dc, "gpiod_get_index" },
	{ 0xcfcce16f, "platform_driver_unregister" },
	{ 0x8693e8d3, "device_destroy" },
	{ 0xec3a392c, "class_destroy" },
	{ 0xb88a2432, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x92997ed8, "_printk" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x7636e2b6, "cdev_init" },
	{ 0xc77ea9f2, "cdev_add" },
	{ 0xb2d13c2b, "class_create" },
	{ 0xd704044d, "device_create" },
	{ 0x5353a139, "__platform_driver_register" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0x8c8569cb, "kstrtoint" },
	{ 0xdcb764ad, "memset" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x3f71a846, "gpiod_get_value" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x38022ea8, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "51036832E1001B46AAA03CD");
