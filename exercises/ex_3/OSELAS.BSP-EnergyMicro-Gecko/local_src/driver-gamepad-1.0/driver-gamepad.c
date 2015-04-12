/*
 * This is a demo Linux kernel module.
 */

// TODO: Denna sidå såg bra ut :D http://www.freesoftwaremagazine.com/articles/drivers_linux
// TODO: Denna sidå såg bedre ut :P http://www.tldp.org/LDP/lkmpg/2.6/html/lkmpg.html#AEN121
// TODO: OOOG denne: http://www.opensourceforu.com/2011/04/character-device-files-creation-operations/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

// declare device number
dev_t devno;
// declare character device
struct cdev my_cdev;
// declare device class
struct class *cl;

/* user program opens the driver */
static int my_open(struct inode *inode, struct file *filp) {
	printk(KERN_INFO "gamepad_driver: open()\n");
	return 0;
}
/* user program closes the driver */
static int my_release(struct inode *inode, struct file *filp) {
	printk(KERN_INFO "gamepad_driver: close()\n");
	return 0;
}
/* user program reads from the driver */
static ssize_t my_read(struct file *filp, char __user *buff, size_t count, loff_t *offp) {
	printk(KERN_INFO "gamepad_driver: read()\n");
	return 0;
}
/* user program writes to the driver */
static ssize_t my_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp) {
	printk(KERN_INFO "gamepad_driver: write()\n");
	return len;
}

// struct for file operations. Needed by the kernel
static struct file operations my_fops = {
	.owner = THIS_MODULE,
	.read = my_read,
	.write = my_write,
	.open = my_open,
	.release = my_release
};

/*
 * gampad_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

static int __init gampad_init(void)
{
	printk("Hello World, here is your module speaking\n");

	// register charachter device number
	if (alloc_chrdev_region(&devno, 0, 1, "gamepad") < 0) return -1;

	// create class sruct
	if ((cl = class_create(THIS_MODULE, "gamepad")) == NULL) {
		unregister_chrdev_region(devno, 1);
		return -1;
	}

	// create device
	if (device_create(cl, NULL, devno, NULL, "gamepad") == NULL) {
		class_destroy(cl);
		unregister_chrdev_region(devno, 1);
		return -1;
	}

	// init character device
	cdev_init(&my_cdev, &my_fops);

	// add device
	if (cdev_add(&my_cdev, devno, 1) == -1) {
		device_destroy(cl, devno);
		class_destroy(cl);
		unregister_chrdev_region(devno, 1);
		return -1;
	}

	return 0;
}

/*
 * gamepad_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit gamepad_cleanup(void)
{
	device_destroy(cl, devno);
	class_destroy(cl);
	unregister_chrdev_region(devno, 1);
	cdev_del(&my_cdev);
	printk("Short life for a small module...\n");
}

module_init(gampad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

