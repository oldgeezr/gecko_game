#include "efm32gg.h"
#include "debug.h"

void setupUART(void) {
  *CMU_HFPERCLKEN0 |= (1 << 4); //Enable UART1
  *UART1_CLKDIV = (152 << 6);
  *UART1_CMD = (1 << 11) | (1 << 10) | (1 << 2) | (1 << 0);
  *UART1_IFC = 0x1FF9;
  *UART1_ROUTE = 0x103;
}

void uartTX(char c)
{
  //while(!(USART1->STATUS & (1 << 6))
    //{
        *UART1_TXDATA = c;
    //}
}
