/*
 * game.c
 *
 *  Created on: 17. apr. 2015
 *      Author: jonzk_000
 */

#include "game.h"


typedef enum _gameState
{
   welcomeScreen,
   welcomeScreenWait,
   gameInit,
   gameRunning,
   gameOver,
   gameOverWait,
} GameState;

static bool gameInitiated = false;
volatile bool gameUpdateFlag, pb0Pressed, pb1Pressed;
volatile GameState state, nextState;
volatile int timeout = 0;
static DISPLAY_Device_t displayDevice;
uint16_t xNow, yNow, barNow;

static void gpioSetup(void);
void GPIO_Unified_IRQ(void);
bool ballMissBar(void);

void GAME_timerEventHandler(void)
{
   static int frameUpdateCount    = 0;
   static int capsenseUpdateCount = 0;

   /* Return at once if init has not been called */
   if (!gameInitiated)
   {
      return;
   }

   if (++frameUpdateCount == FPS)
   {
      frameUpdateCount = 0;

      /* Update state */
      state = nextState;

      /* Set update flag for game */
      gameUpdateFlag = true;
   }

    /* Update wait timer */
    if (timeout > 0)
    {
       timeout--;
    }

   return;
}

void GAME_init(void)
{
	EMSTATUS status;

	/* Initialize the DISPLAY driver. */
	DISPLAY_Init();

	/* Retrieve the properties of the DISPLAY. */
	status = DISPLAY_DeviceGet(DISPLAY_DEVICE_NO, &displayDevice);
	if (DISPLAY_EMSTATUS_OK != status)
		return;

	/* Initialize buttons */
	gpioSetup();

	/* Set initial game state */
	state = welcomeScreen;
	nextState = state;

	gameInitiated = true;

}

void GAME_loop(void)
{
	// Show opening screen for some time or button press

	unsigned int level, bounces;
	int status, ballDir, path, prevPath;

	while(1)
	{
		if (gameUpdateFlag)
	    {
			gameUpdateFlag = false;

	        switch(state)
	        {
	        case welcomeScreen:
	        	GRAPHICS_showWelcomeScreen();
	        	nextState = welcomeScreenWait;
	        	break;


	         case welcomeScreenWait:
	        	 /* Start the game when PB0 or PB1 is pressed */
	        	 if (pb0Pressed || pb1Pressed)
	        	 {
	        		 pb0Pressed = false;
	        		 pb0Pressed = false;
	        		 nextState = gameInit;
	        	 }
	        	 break;

	         case gameInit:
	        	 xNow = SCREEN_RES_X/2;
	        	 yNow = SCREEN_RES_Y - BAR_HEIGHT;
	        	 barNow = SCREEN_RES_X/2;

	        	 GRAPHICS_clearScreen();
	        	 GRAPHICS_printBar(barNow);
	        	 GRAPHICS_printBall(xNow, yNow);

	        	 ballDir = UP;
	        	 path = 0;
	        	 bounces = 0;
	        	 level = 1;

	        	 nextState = gameRunning;

	         case gameRunning:

	        	 /* Check loose-constraints */
	        	 if(!ballMissBar(ballDir)){

	        		 /* Handle bar movement for button presses */
					 if(pb0Pressed && !pb1Pressed)
					 {
						 moveBar(LEFT);
					 }
					 if(pb1Pressed && !pb0Pressed)
					 {
						 moveBar(RIGHT);
					 }

					 /* Handle ball movement */
					 // When ball hits ceiling
					 if(yNow - BALL_RADIUS =< 0){
						 ballDir = DOWN;
						 path = (-1) * prevPath;
						 prevPath = path;
						 moveBall(path, ballDir, level);
					 }
					 // When ball hits left wall
					 else if(xNow - BALL_RADIUS =< 0){
						 path = (-1) * prevPath;
						 prevPath = path;
						 moveBall(path, ballDir, level);
					 }
					 // When ball hits right wall
					 else if(xNow + BALL_RADIUS => SCREEN_RES_X){
						 path = (-1) * prevPath;
						 prevPath = path;
						 moveBall(path, ballDir, level);
					 }
					 // When ball hits bar
					 else if((yNow + BALL_RADIUS => SCREEN_RES_Y - BAR_HEIGHT) && (xNow => barNow - BAR_WIDTH/2 ) && (xNow =< barNow + BAR_WIDTH/2) ){
						 path = int (cos((((xNow - (barNow + (BAR_WIDTH/2)))/(BAR_WIDTH/2)) * 75)));
						 prevPath = path;
						 ballDir = UP;
						 bounces++;
					 }
					 // Moving freely
					 else {
						 moveBall(path, ballDir, level)
					 }

					 if(bounces > 10){
						level++;
						bounces = 0;
					 }
	        	 }
	        	 else {
	        		 nextState = gameOver;
	        	 }
	        	 break;

	         case gameOver:
	        	 /**/
	        	 // Clear screen and finalize
	        	 nextState = gameOverWait;
	         case gameOverWait:
	        	 // Wait
	        	 nextState = welcomeScreen
	        }
	    }
	    else
	    {
	        EMU_EnterEM1();
	    }
	}

	/* if constraints are not vialoted - then: */

	// { 	Handle bar movement


	// 		Handle ball movement

	// 		(Update Score) }

	// Show game over screen for some time


}

void moveBar(int16_t dir)
{
	if(dir < 0 && (barNow - BAR_LENGTH/2) > 0){
		GRAPHICS_clearBar(barNow);
		barNow = barNow - BAR_MOVE;
		GRAPHICS_printBar(barNow);
	}
	else if(dir > 0 && (barNow + BAR_LENGTH/2) < SCREEN_RES_X){
		GRAPHICS_clearBar(barNow);
		barNow = barNow + BAR_MOVE;
		GRAPHICS_printBar(barNow);
	}
}

// Antar 1 pixelbevegelse i Y-akse, 0 er rett opp. Speed 0 er stopp, 1 er laveste hastighet, osv.
void moveBall(int8_t xTrajectory, int8_t yDir, uint16_t lvl)
{
	GRAPHICS_clearBall(xNow, yNow);
	xNow = xNow + xTrajectory;
	yNow = yDir * (yNow + 1*(lvl));
	GRAPHICS_printBall(xNow, yNow);
}

bool ballMissBar(int dir)
{
	if((yNow + BALL_RADIUS + BAR_HEIGHT) > SCREEN_RES_Y && dir > 0){
		return true;
	}
	else {
		return false;
	}
}

int8_t calculateTrajectory(void)




static void gpioSetup(void)
{
  /* Enable GPIO clock */
  CMU_ClockEnable(cmuClock_GPIO, true);

  /* Configure PB0 as input and enable interrupt  */
  GPIO_PinModeSet(BSP_GPIO_PB0_PORT, BSP_GPIO_PB0_PIN, gpioModeInputPull, 1);
  GPIO_IntConfig(BSP_GPIO_PB0_PORT, BSP_GPIO_PB0_PIN, false, true, true);

  /* Configure PB1 as input and enable interrupt */
  GPIO_PinModeSet(BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN, gpioModeInputPull, 1);
  GPIO_IntConfig(BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN, false, true, true);

  NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);

  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);
}

void GPIO_Unified_IRQ(void)
{
  /* Get and clear all pending GPIO interrupts */
  uint32_t interruptMask = GPIO_IntGet();
  GPIO_IntClear(interruptMask);

  /* Act on interrupts */
  if (interruptMask & (1 << BSP_GPIO_PB0_PIN))
  {
    /* PB0 */
    pb0Pressed = true;
  }

  if (interruptMask & (1 << BSP_GPIO_PB1_PIN))
  {
    /* PB1 */
    pb1Pressed = true;
  }
}

void GPIO_EVEN_IRQHandler(void)
{
  GPIO_Unified_IRQ();
}

void GPIO_ODD_IRQHandler(void)
{
  GPIO_Unified_IRQ();
}
