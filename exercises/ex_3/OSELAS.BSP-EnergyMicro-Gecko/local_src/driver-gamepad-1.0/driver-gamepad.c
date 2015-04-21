#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/siginfo.h>
#include <asm/irq.h>

#include "efm32gg.h"

#define NAME                        "gamepad"
#define LED_DEFAULT_OUT             0xFF00
#define DRIVE_MODE_HIGH             2
#define PIN_MODE_PUSH_PULL_DRIVE    0x55555555
#define PIN_MODE_INPUT_PULL_FILTER  0x33333333
#define PORTSEL_PORTC               0x22222222
#define GPIO_EVEN_IRQ               17
#define GPIO_ODD_IRQ                18

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

static                dev_t devno;
static struct         cdev my_cdev;
static struct         class *cl;
static struct         device* dev_create;
struct fasync_struct* async_queue;
void __iomem*         gpio_base;
void __iomem*         gpio_pc_base;
struct                siginfo signal;
struct                task_struct *task;

static irqreturn_t interrupt_handler(int irq, void *dev_id);

// open function for user
int gamepad_open(struct inode *inode, struct file *filp) {

  // enable interruption generation
  request_irq(GPIO_EVEN_IRQ, &interrupt_handler, 0, NAME, NULL);
  request_irq(GPIO_ODD_IRQ, &interrupt_handler, 0, NAME, NULL);

  printk(KERN_INFO "Gamepad driver: open()\n");
  return 0;
}

// release function for user
int gamepad_release(struct inode *inode, struct file *filp) {

  // disable interrupt generation
  free_irq(GPIO_EVEN_IRQ, NULL);
  free_irq(GPIO_ODD_IRQ, NULL);

  printk(KERN_INFO "Gamepad driver: close()\n");
  return 0;
}

// read function for user
ssize_t gamepad_read(struct file *filp, char __user *buff, size_t len, loff_t *offp) {

  int data;
  data = ~ioread32(gpio_pc_base + GPIO_PC_DIN);
  copy_to_user(buff, &data, 1);
  printk(KERN_INFO "Gamepad driver: read()\n");
  return 0;
}
// write function for user
//ssize_t gamepad_write(struct file *filp, const char __user *buff, size_t len, loff_t *offp) {
//
//  printk(KERN_INFO "Gamepad driver: write()\n");
//  return len;
//}

// something
ssize_t gamepad_fasync(int fd, struct file *filp, int mode) {

  return fasync_helper(fd, filp, mode, &async_queue);
}

// struct for file operations. Needed by the kernel
static struct file_operations my_fops = {
  .owner = THIS_MODULE,
  .read = gamepad_read,
  // .write = gamepad_write,
  .open = gamepad_open,
  .release = gamepad_release,
  .fasync = gamepad_fasync
};

static int __init gampad_init(void) {

  int err;

  printk(KERN_INFO "Initialize Gamepad Driver...\n");

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

  // request and map memory
  request_mem_region(GPIO_PA_BASE, GPIO_IFC - GPIO_PA_BASE, NAME);

  // make memory base address for GPIO_PC
  gpio_pc_base = ioremap_nocache(GPIO_PC_BASE, GPIO_IFC - GPIO_PA_BASE);

  // set button pins to input
  iowrite32(PIN_MODE_INPUT_PULL_FILTER, gpio_pc_base + GPIO_PC_MODEL);
  iowrite32(0xFF, gpio_pc_base + GPIO_PC_DOUT);

  // make memory base address for requested memory region
  gpio_base = ioremap_nocache(GPIO_PA_BASE, GPIO_IFC - GPIO_PA_BASE);

  // enable interrupt
  iowrite32(0xFF, gpio_base + GPIO_IEN);
  iowrite32(PORTSEL_PORTC, gpio_base + GPIO_EXTIPSELL);

  // trigger interrupt on btn press
  iowrite32(0xFF, gpio_base + GPIO_EXTIFALL);
  iowrite32(0xFF, gpio_base + GPIO_EXTIRISE);

  // clear interrupt flags
  iowrite32(0xFFFF, gpio_base + GPIO_IFC);

  printk(KERN_INFO "Driver started...\n");
  return 0;
}

static void __exit gamepad_cleanup(void) {

  iounmap(gpio_base);
  iounmap(gpio_pc_base);

  device_destroy(cl, devno);
  class_destroy(cl);
  cdev_del(&my_cdev);
  release_mem_region(GPIO_PA_BASE, GPIO_IFC - GPIO_PA_BASE);
  unregister_chrdev(devno, NAME);

  printk(KERN_INFO "Driver stopped\n");
}

static irqreturn_t interrupt_handler(int irq, void *dev_id) {

  iowrite32(0xFFFF, gpio_base + GPIO_IFC);
  kill_fasync(&async_queue, SIGIO, POLL_IN);
  printk(KERN_INFO "Interrupt handled test...\n");
  return IRQ_HANDLED;
}

module_init(gampad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Driver for Gamepad");
MODULE_LICENSE("GPL");

