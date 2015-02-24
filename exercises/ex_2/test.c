//Test C file
#include <stdint.h>

#define GPIO_PA_BASE 0x40006000
#define GPIO_PB_BASE 0x40006024
#define GPIO_PC_BASE 0x40006048

#define GPIO_PA_CTRL     ((volatile uint32_t*)(GPIO_PA_BASE + 0x00))
#define GPIO_PA_MODEL    ((volatile uint32_t*)(GPIO_PA_BASE + 0x04))
#define GPIO_PA_MODEH    ((volatile uint32_t*)(GPIO_PA_BASE + 0x08))

int main() {
	
	//GPIO_PA_BASE->GPIO_PA_CTRL = 0xffffffff;
	*GPIO_PA_CTRL = 0xffffffff;
	return 0;

}