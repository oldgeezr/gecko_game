#ifndef _GPIO_H_
#define _GPIO_H_

#define LED_MASK  0xFF00

void setupGPIO(void);
void setLed(uint8_t pin);
void clearLed(uint8_t pin);

#endif