#ifndef _TIMER_H_
#define _TIMER_H_

#define LETIMER_IEN_UF      (1 << 2)
#define FCPU                14000000UL

void setupTimer(uint16_t freq);
void setupLowEnergyTimer(void);
void disableLowEnergyTimer(void);
void disableTimer(void);

#endif
