#ifndef _DAC_H_
#define _DAC_H_

#include <stdint.h>

void setupDAC(void);

static inline void writeDAC(uint16_t data) {
	//*DAC0_CH0DATA = data;
	//*DAC0_CH1DATA = data;
	*DAC0_COMBDATA = data;
}

#endif