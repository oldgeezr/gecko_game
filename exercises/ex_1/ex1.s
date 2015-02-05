        .syntax unified

	      .include "efm32gg.s"

	/////////////////////////////////////////////////////////////////////////////
	//
  // Exception vector table
  // This table contains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////

        .section .vectors

	      .long   stack_top               /* Top of Stack                 */
	      .long   _reset                  /* Reset Handler                */
	      .long   dummy_handler           /* NMI Handler                  */
	      .long   dummy_handler           /* Hard Fault Handler           */
	      .long   dummy_handler           /* MPU Fault Handler            */
	      .long   dummy_handler           /* Bus Fault Handler            */
	      .long   dummy_handler           /* Usage Fault Handler          */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* SVCall Handler               */
	      .long   dummy_handler           /* Debug Monitor Handler        */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* PendSV Handler               */
	      .long   dummy_handler           /* SysTick Handler              */

	      /* External Interrupts */
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO even handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO odd handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler

	      .section .text

	/////////////////////////////////////////////////////////////////////////////
	//
	// Reset handler
  // The CPU will start executing here after a reset
	//
	/////////////////////////////////////////////////////////////////////////////

	      .globl  _reset
	      .type   _reset, %function

    .thumb_func
_reset:
        // Enable clk for GPIO
        ldr r1, =CMU_BASE
        ldr r2, [r1, #CMU_HFPERCLKEN0]
        mov r3, #1
        lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
        orr r2, r2, r3
        str r2, [r1, #CMU_HFPERCLKEN0]

        // Set drive strength to 20mA
        ldr r1, =GPIO_PA_BASE
        ldr r2, [r1, #GPIO_CTRL]
        mov r3, #2
        orr r2, r2, r3
        str r2, [r1, #GPIO_CTRL]

        // Configure pin 8-15 as output on PORTA (LEDs)
        ldr r2, [r1, #GPIO_MODEH]
        ldr r3, =0x55555555
        orr r2, r2, r3
        str r2, [r1, #GPIO_MODEH]

        // MOV instruction only support 8 bit. Why?

        // Turn all LEDs off
        mov r2, #0xff
        lsl r2, r2, #8
        str r2, [r1, #GPIO_DOUT]

        // Configure pin 0-7 as input on PORTC (BTNs)
        ldr r1, =GPIO_PC_BASE
        ldr r2, [r1, #GPIO_MODEL]
        ldr r3, =0x33333333
        orr r2, r2, r3
        str r2, [r1, #GPIO_MODEL]

        // Enable pull-up for BTN pins
        mov r2, #0xff
        str r2, [r1, #GPIO_DOUT]

        // Enable interrupt for BTN pins
        ldr r1, =GPIO_BASE
        ldr r2, =0x22222222
        str r2, [r1, #GPIO_EXTIPSELL]

        mov r2, #0xff
        str r2, [r1, #GPIO_EXTIFALL]

        mov r2, #0xff
        str r2, [r1, #GPIO_EXTIRISE]

        mov r2, #0xff
        str r2, [r1, #GPIO_IEN]

        // Enable interrupt handling
        ldr r1, =ISER0
        ldr r2, =0x802
        str r2, [r1, #0]

		//Enable deepsleep mode
		ldr r1, =SCR
		mov r2, #6
		str r2, [r1]

		//bl btn_polling

		//Wait for interrupt
		wfi

	/////////////////////////////////////////////////////////////////////////////
	//
	// Polling function
  // Light LED based on BTN pressed
	//
	/////////////////////////////////////////////////////////////////////////////

	.thumb_func
btn_polling:
		ldr r1, =GPIO_PC_BASE
		ldr r2, =GPIO_PA_BASE
while:
		ldr r3, [r1, #GPIO_DIN]
		lsl r3, r3, #8
		str r3, [r2, #GPIO_DOUT]
		b while

	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////

    .thumb_func
gpio_handler:

		//Clear ISR flag
		ldr r1, =GPIO_BASE
        ldr r2, [r1, #GPIO_IF]
        str r2, [r1, #GPIO_IFC]

        //Light LED based on BTN pressed
        ldr r1, =GPIO_PC_BASE
	    ldr r2, =GPIO_PA_BASE

		ldr r3, [r1, #GPIO_DIN]
	    lsl r3, r3, #8
	    str r3, [r2, #GPIO_DOUT]

        bx lr

	/////////////////////////////////////////////////////////////////////////////

    .thumb_func
dummy_handler:
        b .  // do nothing
