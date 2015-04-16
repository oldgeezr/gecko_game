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
//#include <linux/types.h>
#include <linux/interrupt.h>
//#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
// for char device struct
#include <linux/cdev.h>
// for task struct
#include <linux/sched.h>
// for singal struct
#include <asm/siginfo.h>

#define NAME "gamepad"

// declare character device
struct cdev my_cdev;
// declare device class
struct class *cl;
// declare task struct
// struct task_struct *task;
// Dunno what this does
// struct siginfo signal_info;
// void __iomem *gpio;
// gamepad enable variable
// uint8_t gamepad_en = 0;

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
	return 0;
}

static void __exit gamepad_cleanup(void)
{
	printk("Short life for a small module...\n");
}

module_init(gampad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

