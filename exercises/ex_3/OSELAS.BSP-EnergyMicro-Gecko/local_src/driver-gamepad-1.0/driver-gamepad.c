#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
// for registering char device
#include <linux/fs.h>
// for creating char device class
#include <linux/cdev.h>
#include <linux/device.h>
// for tasks
#include <linux/sched.h>
// for interrupt
#include <linux/interrupt.h>
// for usage of __iomem
#include <asm/io.h>
// for signalinfo
#include <asm/siginfo.h>

#include "efm32gg.h"

#define NAME "gamepad"

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

// declare device number
static dev_t devno;
// declare character device
static struct cdev my_cdev;
// declare device class
static struct class *cl;
// declare __iomem pointer
void __iomem* gpio_base;
// declare signal
struct siginfo signal_info;
// declare task
struct task_struct *task;
// variable for driver enabled
uint8_t driver_enabled = 0;

// function declarations
irqreturn_t interrupt_handler(int irq, void *dev_id, struct pt_regs * regs);

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

	printk(KERN_INFO "Driver version 2\n");

	// register character device
	if (alloc_chrdev_region(&devno, 0, 1, NAME) < 0) return -1;
	else printk(KERN_INFO "Character device registered\n");

	// create class struct
	if ((cl = class_create(THIS_MODULE, NAME)) == NULL) {
		unregister_chrdev_region(devno, 1);
		return -1;
	}

	// create char device
	if (device_create(cl, NULL, devno, NULL, NAME) == NULL) {
		class_destroy(cl);
		unregister_chrdev_region(devno, 1);
		return -1;
	} else printk(KERN_INFO "Characer device created\n");

	// init character device
	cdev_init(&my_cdev, &my_fops);
	my_cdev.owner = THIS_MODULE;

	// add device
	if (cdev_add(&my_cdev, devno, 1) == -1) {
		device_destroy(cl, devno);
		class_destroy(cl);
		unregister_chrdev_region(devno, 1);
		return -1;
	} else printk(KERN_INFO "Driver entry created in /dev\n");

	//Setup signal sending, to trigger interrupts in the game.
	memset(&signal_info, 0, sizeof(struct siginfo));
	signal_info.si_signo = 50;
	signal_info.si_code = SI_QUEUE;

	// request and map memory
	request_mem_region(GPIO_PA_BASE, GPIO_IFC - GPIO_PA_BASE, NAME);

	// makes memory accessable and gets the base value for GPIO operations
	gpio_base = ioremap_nocache(GPIO_PA_BASE, GPIO_IFC - GPIO_PA_BASE);

	printk(KERN_INFO "Read (uint32_t*) gpio_base + ctrl: %x \n", ioread32(gpio_base + GPIO_PA_CTRL));
	printk(KERN_INFO "Read (uint32_t*) gpio_base + modeh: %x \n", ioread32(gpio_base + GPIO_PA_MODEH));

	/// set output pins to high strength
	iowrite32(DRIVE_MODE_HIGH, gpio_base +GPIO_PA_CTRL);
	printk(KERN_INFO "Read (uint32_t*) gpio_base + ctrl: %x \n", ioread32(gpio_base + GPIO_PA_CTRL));
	iowrite32(PIN_MODE_PUSH_PULL_DRIVE, gpio_base + GPIO_PA_MODEH);
	printk(KERN_INFO "Read (uint32_t*) gpio_base + modeh: %x \n", ioread32(gpio_base + GPIO_PA_MODEH));

	// printk(KERN_INFO "Read (uint32_t*) gpio_base + dout: %x \n", ioread32(gpio_base + GPIO_PA_DOUT));

	// set LEDS
	// iowrite32(LED_DEFAULT_OUT, gpio_base + GPIO_PA_DOUT);
	// printk(KERN_INFO "Read (uint32_t*) gpio_base + dout: %x \n", ioread32(gpio_base + GPIO_PA_DOUT));

	gpio_base = ioremap_nocache(GPIO_PC_BASE, GPIO_IFC - GPIO_PA_BASE);

	// set button pins to input
	printk(KERN_INFO "Set btn pins\n");
	iowrite32(PIN_MODE_INPUT_PULL_FILTER, gpio_base + GPIO_PC_MODEL);
	iowrite32(0xFF, gpio_base + GPIO_PC_DOUT);
	printk(KERN_INFO "Btn pins set\n");

	// enable interrupt
	iowrite32(0xFF, gpio_base + GPIO_IEN);
	iowrite32(PORTSEL_PORTC, gpio_base + GPIO_EXTIPSELL);
	printk(KERN_INFO "Interrupt enabled\n");

	// trigger interrupt on btn press
	iowrite32(0xFF, gpio_base + GPIO_EXTIFALL);
	printk(KERN_INFO "Interrupt on bt press\n");

	// clear interrupt flags
	iowrite32(0xFFFF, gpio_base + GPIO_IFC);
	printk(KERN_INFO "Clear interrupt flags\n");

	//Enable interruption generation
	request_irq(17, (irq_handler_t)interrupt_handler, 0, NAME, &my_cdev);
	request_irq(18, (irq_handler_t)interrupt_handler, 0, NAME, &my_cdev);
	printk(KERN_INFO "Interrupt requested\n");


	printk(KERN_INFO "Driver started v1\n");
	return 0;
}

static void __exit gamepad_cleanup(void) {
	device_destroy(cl, devno);
	class_destroy(cl);
	cdev_del(&my_cdev);
	//Release interrupt handlers
	free_irq(17, NULL);
	free_irq(18, NULL);

	//Release memory
	iounmap(gpio_base);
	release_mem_region(GPIO_PA_BASE, GPIO_IFC - GPIO_PA_BASE);
	unregister_chrdev(devno, NAME);
	printk(KERN_INFO "Driver unregistered\n");
	printk(KERN_INFO "Driver stopped\n");
}

irqreturn_t interrupt_handler(int irq, void *dev_id, struct pt_regs * regs) {
	printk(KERN_INFO "GOTH interrupt\n");
	//Read the button status
	uint32_t buttons = ioread32(gpio_base + GPIO_PC_DIN);

	//Set the signal value to the reversed button value(Because they are active low)
	signal_info.si_int = ~buttons;

	//Resets the interrupt
	iowrite32(0xFFFF, gpio_base + GPIO_IFC);

	//Checks if everything is up and running and sends the signal to the game.
	if(driver_enabled)
	{
		int status = send_sig_info(50, &signal_info, task);
		if (status < 0)
		{
			printk(KERN_INFO "Unable to send interrupt\n");
			return -1;
		}
	}
	else
	{
		printk(KERN_INFO "Driver not enabled\n");
		return -1;
	}

	return IRQ_HANDLED;
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

