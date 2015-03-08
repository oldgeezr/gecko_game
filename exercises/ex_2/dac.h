#ifndef _DAC_H_
#define _DAC_H_

#include <stdint.h>

void setupDAC(uint8_t PRS);
void disableDAC(void);
void writeDAC(uint16_t data);

#endif
