
#include "dac.h"
#include <stdbool.h>

void setupDAC(void)
{
   *CMU_HFPERCLKEN0 |= HFPER_DAC0;
   *DAC0_CTRL = OUTMODE_PIN | PRESC; // | (1 << 6);
   *DAC0_CH0CTRL = (1 << 0)| (1 << 2);
   *DAC0_CH1CTRL = (1 << 0)| (1 << 2);
}

void disableDAC(void) 
{
	*CMU_HFPERCLKEN0 &= ~(HFPER_DAC0);
	*DAC0_CH0CTRL = 0;
    *DAC0_CH1CTRL = 0;
    *DAC0_CTRL = 0;
}

void setupDMA(void) 
{
	*CMU_HFCORECLKEN0 |= (1 << 0); //Eable DMA clock
    *DMA_CTRLBASE = (uint32_t)(dmaControlBlock); //Init ctrl base
    *DMA_CONFIG = 1; //Enable DMA

	*DMA_CH0_CTRL = (0x0A << 16); //Select DAC0 as source
	*DMA_CH0_LOOP |= (1 << 16); //Enable loop
	*DMA_CH0_LOOP |= (DMA_BUFFER_SIZE - 1); //Set loop size

	configDMA_t *primDescr;
	configDMA_t *descr;

	primDescr = (configDMA_t *)(*DMA_CTRLBASE);
	descr = (configDMA_t *)(*DMA_ALTCTRLBASE);
	descr = primDescr;

	controlData_t control;

	control.dst_inc = NO_INCREMENT;
	control.dst_size = WORD;
	control.src_inc = WORD;
	control.src_size = WORD;
	control.n_minus_1 = (DMA_BUFFER_SIZE - 1);
	control.cycle_ctrl = CYCLE_CTRL_BASIC;

	primDescr->src_data_end_ptr = (void *)((uint32_t)dma_buffer + (control.n_minus_1 << 2));
	primDescr->dst_data_end_ptr = (void *)((uint32_t)DAC0_COMBDATA);
	primDescr->channel_cfg = (control.dst_inc << 30) | (control.dst_size << 28) | (control.src_inc << 26) 
					   | (control.src_size << 24) | (control.n_minus_1 << 4) | (control.cycle_ctrl << 0);

	//*DMA_CHUSEBURSTS = 1;
	//*DMA_REQMASKC = 1;
	*DMA_CHALTC = 1;
	*DMA_CHENS = 1;
}

void setupPRS(void)
{
	*CMU_HFPERCLKEN0 |= HFPER_PRS;
	//*PRS_CH0_CTRL = (0x34 << 16); //source LETIMER0
	*PRS_CH0_CTRL = (0x1D << 16) | (0x1 << 0); //source TIMER1 and overflow interrupt
}

void setupUART(void) {
	*CMU_HFPERCLKEN0 |= (1 << 4); //Enable UART1
	*UART1_CLKDIV = (152 << 6);                          
  	*UART1_CMD = (1 << 11) | (1 << 10) | (1 << 2) | (1 << 0); 
  	*UART1_IFC = 0x1FF9;                                     
  	*UART1_ROUTE = 0x103;                                     
}
