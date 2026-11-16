// FILE: hello.c - Hello world kernel module
#include <linux/module.h>

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello, world\n");
    return 0;
}
static void __exit hello_exit(void)
{
    printk(KERN_INFO "Goodbye, cruel world\n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_DESCRIPTION("Hello World Kernel Module");
MODULE_AUTHOR("Peter HM <phm@ece.au.dk>");
MODULE_LICENSE("GPL");