#ifndef _INTERRUPT_HANDLERS_H_
#define _INTERRUPT_HANDLERS_H_

#include <stdint.h>

#define SWITCH_1  0xfe
#define SWITCH_2  0xfd
#define SWITCH_3  0xfb
#define SWITCH_4  0xf7
#define SWITCH_5  0xef
#define SWITCH_6  0xdf
#define SWITCH_7  0xbf
#define SWITCH_8  0x7f

uint16_t global_freq = A;

void __attribute__ ((interrupt)) DMA_IRQHandler();
void __attribute__ ((interrupt)) TIMER1_IRQHandler();
void __attribute__ ((interrupt)) LETIMER0_IRQHandler();
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler();
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler();

#endif
