#ifndef _TIMER_H_
#define _TIMER_H_

#define FCPU                14000000UL
#define SAMPLE_FREQ         44100

void setupTimer            (uint16_t freq);
void setupLowEnergyTimer   (void);
void disableLowEnergyTimer (void);
void disableTimer          (void);

#endif
