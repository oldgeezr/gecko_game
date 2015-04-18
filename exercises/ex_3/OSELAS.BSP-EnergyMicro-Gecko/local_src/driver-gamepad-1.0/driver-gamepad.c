#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/siginfo.h>
#include <asm/irq.h>

#include "efm32gg.h"

#define NAME												"gamepad"
#define LED_DEFAULT_OUT             0xFF00
#define DRIVE_MODE_HIGH             2
#define PIN_MODE_PUSH_PULL_DRIVE    0x55555555
#define PIN_MODE_INPUT_PULL_FILTER  0x33333333
#define PORTSEL_PORTC               0x22222222
#define GPIO_EVEN_IRQ								17
#define GPIO_ODD_IRQ								18

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

static				dev_t devno;
static struct cdev my_cdev;
static struct class *cl;
static struct device* dev_create;
void __iomem* gpio_base;
void __iomem* gpio_irq_base;
struct				siginfo signal;
struct				task_struct *task;
uint8_t				driver_en = 0;

// function declarations
static irqreturn_t interrupt_handler(int irq, void *dev_id);

/* user program opens the driver */
int gamepad_open(struct inode *inode, struct file *filp) {
	printk(KERN_INFO "gamepad_driver: open()\n");
	return 0;
}
/* user program closes the driver */
int gamepad_release(struct inode *inode, struct file *filp) {

	driver_en = 0;
	printk(KERN_INFO "gamepad_driver: close()\n");
	return 0;
}
/* user program reads from the driver */
ssize_t gamepad_read(struct file *filp, char __user *buff, size_t len, loff_t *offp) {
	printk(KERN_INFO "gamepad_driver: read()\n");
	return 0;
}
/* user program writes to the driver */
ssize_t gamepad_write(struct file *filp, const char __user *buff, size_t len, loff_t *offp) {
	printk(KERN_INFO "gamepad_driver: write()\n");
	return len;
}

// struct for file operations. Needed by the kernel
static struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.read = gamepad_read,
	.write = gamepad_write,
	.open = gamepad_open,
	.release = gamepad_release
};

static int __init gampad_init(void) {

	int err;

	printk(KERN_INFO "Initialize Gamepad Driver Version 0...\n");

	// allocate region for character device
	err = alloc_chrdev_region(&devno, 0, 1, NAME);

	if (err < 0) {
		return -1;
	} else {
		printk(KERN_INFO "Character device registerd...\n");
	}

	// create class struct for device
	cl = class_create(THIS_MODULE, NAME);

	if (cl == NULL) {
		unregister_chrdev_region(devno, 1);
		return -1;
	}

	// create character device in user space
	dev_create = device_create(cl, NULL, devno, NULL, NAME);

	if (dev_create == NULL) {
		class_destroy(cl);
		unregister_chrdev_region(devno, 1);
		return -1;
	} else {
		printk(KERN_INFO "Character device created...\n");
	}

	// initialize character device
	cdev_init(&my_cdev, &my_fops);
	// my_cdev.owner = THIS_MODULE;

	// add device to the kernel
	err = cdev_add(&my_cdev, devno, 1);

	if (err < 0) {
		device_destroy(cl, devno);
		class_destroy(cl);
		unregister_chrdev_region(devno, 1);
		return -1;
	} else {
		printk(KERN_INFO "Driver entry created in /dev\n");
	}

	// allocate memory for signal
	memset(&signal, 0, sizeof(struct siginfo));
	signal.si_signo = 50;
	signal.si_code = SI_QUEUE;

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
	gpio_irq_base = ioremap_nocache(GPIO_PA_BASE, GPIO_IFC - GPIO_PA_BASE);

	// set button pins to input
	printk(KERN_INFO "Set btn pins\n");
	iowrite32(PIN_MODE_INPUT_PULL_FILTER, gpio_base + GPIO_PC_MODEL);
	iowrite32(0xFF, gpio_base + GPIO_PC_DOUT);
	printk(KERN_INFO "Btn pins set\n");

	// enable interrupt
	iowrite32(0xFF, gpio_irq_base + GPIO_IEN);
	iowrite32(PORTSEL_PORTC, gpio_irq_base + GPIO_EXTIPSELL);
	printk(KERN_INFO "Interrupt enabled\n");

	// trigger interrupt on btn press
	iowrite32(0xFF, gpio_irq_base + GPIO_EXTIFALL);
	printk(KERN_INFO "Interrupt on bt press\n");

	// clear interrupt flags
	iowrite32(0xFFFF, gpio_irq_base + GPIO_IFC);
	printk(KERN_INFO "Clear interrupt flags\n");

	//Enable interruption generation
	request_irq(GPIO_EVEN_IRQ, &interrupt_handler, 0, NAME, NULL);
	request_irq(GPIO_ODD_IRQ, &interrupt_handler, 0, NAME, NULL);
	printk(KERN_INFO "Interrupt requested\n");

	// set driver enabled flag
	driver_en = 1;

	printk(KERN_INFO "Driver started v1\n");
	return 0;
}

static void __exit gamepad_cleanup(void) {
	device_destroy(cl, devno);
	class_destroy(cl);
	cdev_del(&my_cdev);
	//Release interrupt handlers
	free_irq(GPIO_EVEN_IRQ, NULL);
	free_irq(GPIO_ODD_IRQ, NULL);

	//Release memory
	iounmap(gpio_base);
	iounmap(gpio_irq_base);
	release_mem_region(GPIO_PA_BASE, GPIO_IFC - GPIO_PA_BASE);
	unregister_chrdev(devno, NAME);
	printk(KERN_INFO "Driver unregistered\n");
	printk(KERN_INFO "Driver stopped\n");
}

static irqreturn_t interrupt_handler(int irq, void *dev_id) {

	printk(KERN_INFO "Recorded button interrupt...\n");
	//Read the button status
	uint32_t buttons;
	buttons = ioread32(gpio_base + GPIO_PC_DIN);

	//Set the signal value to the reversed button value(Because they are active low)
	signal.si_int = ~buttons;

	//Resets the interrupt
	iowrite32(0xFFFF, gpio_irq_base + GPIO_IFC);

	//Checks if everything is up and running and sends the signal to the game.
	if(driver_en) {
		printk(KERN_INFO "Interrupt was handled...\n");
		int err;
		err = send_sig_info(50, &signal, task);
		if (err < 0) {
			printk(KERN_INFO "Interrupt passed to task...\n");
			return -1;
		}
	} else {
		printk(KERN_INFO "Driver not enabled\n");
		return -1;
	}

	return IRQ_HANDLED;
}

module_init(gampad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Driver for Gamepad");
MODULE_LICENSE("GPL");

