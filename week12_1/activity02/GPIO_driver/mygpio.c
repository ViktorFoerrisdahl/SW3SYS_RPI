#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/of_gpio.h>
#include <linux/module.h>
#include <linux/fs.h>

#define DRIVER_NAME "mygpio" // Must start with "my"
#define DEVICE_NAME "but0"   // Name of specific device

static dev_t dev_num;
static struct cdev mygpio_cdev;
static struct class *mygpio_class;
static struct gpio_desc *gpio_btn;


static ssize_t mygpio_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    char kbuf[16]; // Buffer allocated in kernel
    int value;

    // Set value
    // value = 23;

    //read value
    value = gpiod_get_value(gpio_btn);
    
    // Format string
    int n = snprintf(kbuf, sizeof(kbuf), "%d\n", value);

    // Copy string to user space
    if (copy_to_user(buf, kbuf, n))
        return -EFAULT;

    return n; // Return length of string
}

static ssize_t mygpio_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    char kbuf[16];
    int value;

    // Return error if data written is bigger than kernel buffer
    if (len >= sizeof(kbuf))
        return -EINVAL;

    // Copy data from user space to kernel space
    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;

    // Append a zero-termination to ascii buffer (C-string)
    kbuf[len] = '\0';

    // Convert string to int value (aut base)
    if (kstrtoint(kbuf, 0, &value))
        return -EINVAL;

    // Use value for something...
    printk(KERN_INFO "Recieved %d from user_space\n", value);

    // Return that written length = requested write length => Everything ok
    return len;
}

// Define which file functions to use for file operations
static struct file_operations mygpio_fops = {
    .owner = THIS_MODULE,
    .read  = mygpio_read,
    .write = mygpio_write,
};

static int mygpio_probe(struct platform_device *pdev)
{
    int err = 0;
    struct device *dev = &pdev->dev;

    // Activity 3.2: Allocate GPIO descriptors from Device tree entry
    gpio_btn = gpiod_get_index(dev, "my", 0, GPIOD_IN);

    // Activity 3.2: Check that gpio_btn is non-zero
    if (gpio_btn == 0)
    {
        return -1;
    }

    return err;
}

static void mygpio_remove(struct platform_device *pdev)
{
    // Activity 3.2: Cleanup what was allocated in Probe
    gpiod_put(gpio_btn);
}

static const struct of_device_id of_mygpio_platform_device_match[] = {
   { .compatible = "au-ece, my-gpio", }, {},
 };

 static struct platform_driver mygpio_platform_driver = {
 	.probe      = mygpio_probe,
 	.remove	    = mygpio_remove,
 	.driver     = {
    .name   = "mygpio",
 		.of_match_table = of_mygpio_platform_device_match,
 		.owner = THIS_MODULE,
 	},
 };

/* 
    mygpio init
    This is boiler-plate code for a character driver.
    It allocates an available Major number and uses
    only one Minor number, 0.
    Creates a class and one device for minor number 0
*/
static int mygpio_init(void)
{
    int ret;

    // Request an available Major number and Minor numbers [0..255]
    ret = alloc_chrdev_region(&dev_num, 0, 255, DRIVER_NAME);
    if (ret)
        return ret;

    // Initialize character driver with the fileoperations in mygpio_fops and add the driver to Linux's list of drivers 
    cdev_init(&mygpio_cdev, &mygpio_fops);
    ret = cdev_add(&mygpio_cdev, dev_num, 255);
    if (ret)
        goto unregister_region;

    // Create a class in /sys/class (needed for devices)
    mygpio_class = class_create(DRIVER_NAME);
    if (IS_ERR(mygpio_class)) {
        ret = PTR_ERR(mygpio_class);
        goto del_cdev;
    }

    // Create a device: 
    // Major = the one allocated with `alloc_chrdev_region`
    // Minor = 0
    // Name = <DRIVER_NAME>-<DEVICE_NAME> eg /dev/mygpio-but0
    device_create(mygpio_class, NULL, MKDEV(MAJOR(dev_num), MINOR(0)), NULL, "%s-%s", DRIVER_NAME, DEVICE_NAME);

    // Register driver as a Platform driver
    // (Driver for on-board devices described in a device tree)
    ret = platform_driver_register(&mygpio_platform_driver);
    if(ret)
        goto cleanup_device;

    printk(KERN_INFO "%s: loaded (major=%d)\n", DRIVER_NAME, MAJOR(dev_num));
    return 0;

cleanup_device:    
    device_destroy(mygpio_class, dev_num);
    class_destroy(mygpio_class);
del_cdev:
    cdev_del(&mygpio_cdev);
unregister_region:
    unregister_chrdev_region(dev_num, 255);
    return ret;
}

static void mygpio_exit(void)
{
    // Exit cleanup is reverse-order from init!!!
    platform_driver_unregister(&mygpio_platform_driver);
    device_destroy(mygpio_class, dev_num);
    device_destroy(mygpio_class, dev_num+1);
    class_destroy(mygpio_class);
    cdev_del(&mygpio_cdev);
    unregister_chrdev_region(dev_num, 255);
    printk(KERN_INFO "%s: unloaded\n", DRIVER_NAME);
}

module_init(mygpio_init);
module_exit(mygpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Peter HM <phm@ece.au.dk>");
MODULE_DESCRIPTION("Minimal GPIO char driver");