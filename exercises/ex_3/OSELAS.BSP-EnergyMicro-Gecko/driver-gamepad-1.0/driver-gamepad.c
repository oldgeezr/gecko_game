#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
static int __init gampad_init(void) {

	printk(KERN_INFO "Driver started\n");
	return 0;
}

static void __exit gamepad_cleanup(void) {
	printk(KERN_INFO "Driver stopped\n");
}

//static inline void mem_write(void* base, uint16_t register_, uint32_t value) {
//	*(volatile uint32_t*)((uint32_t)base + register_) = value;
//}
//
//static inline uint32_t mem_read(void* base, uint16_t register_) {
//	return *(volatile uint32_t*)((uint32_t)base + register_);
//}

module_init(gampad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Driver for Gamepad");
MODULE_LICENSE("GPL");
