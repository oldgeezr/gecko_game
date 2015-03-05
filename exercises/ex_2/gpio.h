#ifndef _GPIO_H_
#define _GPIO_H_

#define LED_MASK  0xFF00

#define DRIVE_MODE_HIGH             2
#define PIN_MODE_PUSH_PULL_DRIVE    0x55555555
#define PIN_MODE_INPUT_PULL_FILTER  0x33333333
#define PORTSEL_PORTC               0x22222222

void setupGPIO(void);
void setLed(uint8_t pin);
void clearLed(uint8_t pin);

#endif
