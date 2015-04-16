#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
// for registering char device
#include <linux/fs.h>
// for creating char device class
#include <linux/cdev.h>
#include <linux/device.h>
// for usage of __iomem
// #include <asm/io.h>

#include "efm32gg.h"

#define NAME "gamepad"

// declare device number
//static dev_t devno;
// declare character device
//static struct cdev my_cdev;
// declare device class
//static struct class *cl;
//// declare __iomem pointer
//void __iomem* gpio_base;
//
//static inline void mem_write(void* base, uint16_t register_, uint32_t value);
//static inline uint32_t mem_read(void* base, uint16_t register_);

/* user program opens the driver */
int gamepad_open(struct inode *inode, struct file *filp) {
	printk(KERN_INFO "gamepad_driver: open()\n");
	return 0;
}
/* user program closes the driver */
int gamepad_release(struct inode *inode, struct file *filp) {
	printk(KERN_INFO "gamepad_driver: close()\n");
	return 0;
}
/* user program reads from the driver */
ssize_t gamepad_read(struct file *filp, char __user *buff, size_t count, loff_t *offp) {
	printk(KERN_INFO "gamepad_driver: read()\n");
	return 0;
}
/* user program writes to the driver */
ssize_t gamepad_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp) {
	printk(KERN_INFO "gamepad_driver: write()\n");
	return count;
}

// struct for file operations. Needed by the kernel
static struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.read = gamepad_read,
	.write = gamepad_write,
	.open = gamepad_open,
	.release = gamepad_release
	//.lseek=
	//.readdir =
	//.select =
	//.ioctl =
	//.fsyn =
	//.mmap =
	//.fasync =
	//.check_media_change =
	//.revalidate =
};

static int __init gampad_init(void) {

	//// request and map memory
	//check_mem_region(GPIO_PA_BASE, GPIO_SIZE);
	//request_mem_region(GPIO_PA_BASE, GPIO_SIZE, NAME);

	//// makes memory accessable and gets the base value for GPIO operations
	//gpio_base = ioremap_nocache(GPIO_PA_BASE, GPIO_SIZE);

	//// set output pins to high strength
	///mem_write(gpio_base, GPIO_PA_CTRL, DRIVE_MODE_HIGH);
	///mem_write(gpio_base, GPIO_PA_CTRL, PIN_MODE_PUSH_PULL_DRIVE);
	//// set LEDS
	//// mem_write(gpio_base, GPIO_PA_CTRL, LED_DEFAULT_OUT);

	// register character device
	//if (alloc_chrdev_region(&devno, 0, 1, NAME) < 0) return -1;
	//else printk(KERN_INFO "Character device registered\n");

	//// create class struct
	//if ((cl = class_create(THIS_MODULE, NAME)) == NULL) {
	//	unregister_chrdev_region(devno, 1);
	//	return -1;
	//}

	//// create char device
	//if (device_create(cl, NULL, devno, NULL, NAME) == NULL) {
	//	class_destroy(cl);
	//	unregister_chrdev_region(devno, 1);
	//	return -1;
	//} else printk(KERN_INFO "Characer device created\n");

	//// init character device
	//cdev_init(&my_cdev, &my_fops);
	//my_cdev.owner = THIS_MODULE;

	//// add device
	//if (cdev_add(&my_cdev, devno, 1) == -1) {
	//	device_destroy(cl, devno);
	//	class_destroy(cl);
	//	unregister_chrdev_region(devno, 1);
	//	return -1;
	//} else printk(KERN_INFO "Driver entry created in /dev\n");

	printk(KERN_INFO "Driver started\n");
	return 0;
}

static void __exit gamepad_cleanup(void) {
	//device_destroy(cl, devno);
	//class_destroy(cl);
	//cdev_del(&my_cdev);
	//unregister_chrdev(devno, NAME);
	printk(KERN_INFO "Driver unregistered\n");
	printk(KERN_INFO "Driver stopped\n");
}

//static inline void mem_write(void* base, uint16_t register_, uint32_t value) {
//	*(volatile uint32_t*)(base + register_) = value;
//}
//
//static inline uint32_t mem_read(void* base, uint16_t register_) {
//	return *(volatile uint32_t*)(base + register_);
//}

module_init(gampad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Driver for Gamepad");
MODULE_LICENSE("GPL");

