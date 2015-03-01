
#include "dac.h"
#include <stdbool.h>

void setupDAC(void)
{
   *CMU_HFPERCLKEN0 |= HFPER_DAC0;
   *DAC0_CTRL = OUTMODE_PIN | PRESC;
   *DAC0_CH0CTRL = (1 << 0) | (1 << 2);
   *DAC0_CH1CTRL = (1 << 0) | (1 << 2);
}

void disableDAC(void) 
{
	*CMU_HFPERCLKEN0 &= ~(HFPER_DAC0);
}

void setupDMA(void) 
{
	*DMA_CH0_CTRL = (0x0A << 16); //DAC0
	*DMA_CH0_LOOP = (1 << 16); //Enable looping

	configDMA_t config;
	config.src_data_end_ptr = (uint32_t *)(dma_buffer + DMA_BUFFER_SIZE);
	config.dst_data_end_ptr = ((uint32_t *)DAC0_COMBDATA);

	controlData_t control;
	control.dst_inc = NO_INCREMENT;
	control.dst_size = WORD;
	control.src_inc = WORD;
	control.src_size = WORD;
	control.n_minus_1 = (DMA_BUFFER_SIZE - 1);
	control.cycle_ctrl = CYCLE_CTRL_BASIC;

	config.channel_cfg = (control.dst_inc << 30) | (control.dst_size << 28) | (control.src_inc << 26) 
					   | (control.src_size << 24) | (control.n_minus_1 << 4) | (control.cycle_ctrl << 0);

	//SrcEngPtr
	*(DMA_CTRLBASE + 0x000) = *(config.src_data_end_ptr);
	//DstEndPtr
	*(DMA_CTRLBASE + 0x004) = *(config.dst_data_end_ptr);
	//ch_cfg
	*(DMA_CTRLBASE + 0x008) = config.channel_cfg;

	*DMA_CONFIG = true;
	*DMA_CHUSEBURSTS = true;
	*DMA_REQMASKC = true;
	*DMA_CHALTC = true;
	*DMA_CHENS = true;
}

void setupPRS(void)
{
	*CMU_HFPERCLKEN0 |= HFPER_PRS;
	*PRS_CH0_CTRL = (0x34 << 16); //source LETIMER0
}

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
