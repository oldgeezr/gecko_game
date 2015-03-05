#ifndef _DAC_H_
#define _DAC_H_

#include <stdint.h>

#define DIFFERENTIAL  (1 << 0)
#define OUTMODE_PIN   (0x1 << 4)
#define PRESC 		    (0x5 << 16)
#define HFPER_DAC0    (1 << 17)
#define PRSCH0        (1 << 0)

void setupDAC(uint8_t PRS);
void disableDAC(void);
void writeDAC(uint16_t data);

#endif
