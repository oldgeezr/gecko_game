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
        // Enable GPIO clk
        ldr r1, cmu_base_addr
        ldr r2, [r1, #CMU_HFPERCLKEN0]
        mov r3, #1
        lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
        orr r2, r2, r3
        str r2, [r1, #CMU_HFPERCLKEN0]

        // Enable drive strength
        ldr r1, gpio_pa_base_addr
        ldr r2, [r1, #GPIO_CTRL]
        ldr r3, =0x2
        orr r2, r2, r3
        str r2, [r1, #GPIO_CTRL]

        // Set pin diretion in MODEH (8-15)
        ldr r2, [r1, #GPIO_MODEH]
        ldr r3, =0x55555555
        orr r2, r2, r3
        str r2, [r1, #GPIO_MODEH]

        // MOV instruction only support 8 bit. Why?

        // Set LEDS
        // mov r2, #0x55
        // lsl r2, r2, #8
        // str r2, [r1, #GPIO_DOUT]

        // Enable input and pull direction by DOUT
        ldr r1, gpio_pc_base_addr
        ldr r2, [r1, #GPIO_MODEL]
        ldr r3, =0x33333333
        orr r2, r2, r3
        str r2, [r1, #GPIO_MODEL]

        // Enable pull-up
        ldr r2, =0xff
        str r2, [r1, #GPIO_DOUT]

        // Enable interrupt on port C
        ldr r1, gpio_base_addr
        ldr r2, [r1, #GPIO_EXTIPSELL]
        ldr r3, =0x22222222
        orr r2, r2, r3
        str r2, [r1, #GPIO_EXTIPSELL]

        ldr r2, [r1, #GPIO_EXTIRISE]
        ldr r3, =0xff
        orr r2, r2, r3
        str r2, [r1, #GPIO_EXTIRISE]

        ldr r2, [r1, #GPIO_EXTIFALL]
        ldr r3, =0xff
        orr r2, r2, r3
        str r2, [r1, #GPIO_EXTIFALL]

        ldr r2, [r1, #GPIO_IEN]
        ldr r3, =0xff
        orr r2, r2, r3
        str r2, [r1, #GPIO_IEN]

        // Enable interrupt handling
        ldr r1, nvic_iser0_addr
        ldr r2, =0x802
        str r2, [r1, #0]

// Read btn state and update leds accordingly
btn_poll:

        //ldr r3, gpio_pa_base_addr
        //ldr r2, [r1, #GPIO_DIN]
        //lsl r2, r2, #8
        //str r2, [r3, #GPIO_DOUT]
        b btn_poll

cmu_base_addr:
        .long CMU_BASE

// Base address for LEDS
gpio_pa_base_addr:
        .long GPIO_PA_BASE

// Base address for GPIO
gpio_base_addr:
        .long GPIO_BASE

// Base address for BTNS
gpio_pc_base_addr:
        .long GPIO_PC_BASE

// Address for ISER0
nvic_iser0_addr:
        .long ISER0

	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////

        .thumb_func
gpio_handler:

        ldr r3, gpio_pa_base_addr
        ldr r1, gpio_base_addr
        ldr r2, [r1, #GPIO_IF]
        lsl r2, r2, #8
        str r2, [r3, #GPIO_DOUT]
        str r2, [r1, #GPIO_IFC]

	/////////////////////////////////////////////////////////////////////////////

        .thumb_func
dummy_handler:
        b .  // do nothing
