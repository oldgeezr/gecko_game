#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
// for registering char device
#include <linux/fs.h>
// for creating char device class
#include <linux/cdev.h>

#define NAME "gamepad"

// declare device number
dev_t devno;
// declare character device
struct cdev my_cdev;
// declare device class
struct class *cl;

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
	// should probably return the length of the write?
	return 0;
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

static int __init gampad_init(void)
{
	// register charachter device number
	if (alloc_chrdev_region(&devno, 0, 1, NAME) < 0) return -1;
	else printk(KERN_INFO "Driver registered\n");

	// create class struct
	if ((cl = class_create(THIS_MODULE, NAME)) == NULL) {
		unregister_chrdev_region(devno, 1);
		return -1;
	}

	// create device
	if (device_create(cl, NULL, devno, NULL, NAME) == NULL) {
		class_destroy(cl);
		unregister_chrdev_region(devno, 1);
		return -1;
	} else printk(KERN_INFO "Driver put in /dev\n");

	printk(KERN_INFO "Start driver\n");
	return 0;
}

static void __exit gamepad_cleanup(void)
{
	unregister_chrdev(89, NAME);
	printk(KERN_INFO "Driver unregistered\n");
	printk(KERN_INFO "Exit driver\n");
}

module_init(gampad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

