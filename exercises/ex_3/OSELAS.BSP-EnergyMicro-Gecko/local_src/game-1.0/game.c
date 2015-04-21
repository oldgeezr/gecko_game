//`AC_DEFINE(_GNU_SOURCE)'
//     GNU packages should normally include this line before any other
//     feature tests.  This defines the macro `_GNU_SOURCE' when
//     compiling, which directs the libc header files to provide the
//     standard GNU system interfaces including all GNU extensions.  If
//     this macro is not defined, certain GNU extensions may not be
//     available.

#ifndef _POSIX_C_SOURCE
//#define _POSIX_C_SOURCE 1
#define _GNU_SOURCE 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <fcntl.h>

#include "game.h"
//#include "display.h"
//
//color_t blue   = {31,0,0},
//        red    = {0,0,31},
//        green  = {0,63,0},
//        yellow = {0,63,31},
//        cyan   = {31,63,0},
//        pink   = {31,0,31},
//        white  = {31,63,31},
//        black  = {0,0,0};

#define true 1
#define false 0

#define SW0 0
#define SW1 1
#define SW7 64

typedef char bool;

int descr;
int buff;

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
//static DISPLAY_Device_t displayDevice;
uint16_t xNow, yNow, barNow;

bool ballMissBar(int dir);
void gpioSetup();
void interrupt_handler(int signo);
void moveBar(int16_t dir);
void moveBall(int8_t xTrajectory, int8_t yDir, uint16_t lvl);
bool ballMissBar(int dir);

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
  /* Initialize the DISPLAY driver. */
  //DISPLAY_Init();
  display_setup();

  ///* Retrieve the properties of the DISPLAY. */
  //status = DISPLAY_DeviceGet(DISPLAY_DEVICE_NO, &displayDevice);
  //if (DISPLAY_EMSTATUS_OK != status)
  //  return;

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
            pb1Pressed = false;
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
            if(yNow - BALL_RADIUS <= 0){
              ballDir = DOWN;
              path = (-1) * prevPath;
              prevPath = path;
              moveBall(path, ballDir, level);
            }
            // When ball hits left wall
            else if(xNow - BALL_RADIUS <=0){
              path = (-1) * prevPath;
              prevPath = path;
              moveBall(path, ballDir, level);
            }
            // When ball hits right wall
            else if(xNow + BALL_RADIUS >= SCREEN_RES_X){
              path = (-1) * prevPath;
              prevPath = path;
              moveBall(path, ballDir, level);
            }
            // When ball hits bar
            else if((yNow + BALL_RADIUS >= SCREEN_RES_Y - BAR_HEIGHT) && (xNow >= barNow - BAR_LENGTH/2 ) && (xNow <= barNow + BAR_LENGTH/2) ){
              // A bit sketchy
              path = (int)(cos((((xNow - (barNow + (BAR_LENGTH/2)))/(BAR_LENGTH/2)) * 75)));
              prevPath = path;
              ballDir = UP;
              bounces++;
            }
            // Moving freely
            else {
              moveBall(path, ballDir, level);
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
          break;
        case gameOverWait:
          // Wait
          nextState = welcomeScreen;
          break;
      }
    }
    else
    {
      sleep(1);
    }
  }

  /* if constraints are not vialoted - then: */

  // {  Handle bar movement


  //    Handle ball movement

  //    (Update Score) }

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

void interrupt_handler(int signo) {

  read((descr), &buff, 1);
  printf("type: %d reading %d\n", signo, buff);

  switch (buff) {
    case SW0:
      pb0Pressed = false;
      pb1Pressed = false;
      break;
    case SW1:
      pb0Pressed = true;
      pb1Pressed = false;
      break;
    case SW7:
      pb0Pressed = false;
      pb1Pressed = true;
      break;
  }

  printf("pb0 %d, pb1 %d\n", pb0Pressed, pb1Pressed);
}

void gpioSetup() {

  int oflags;
  descr = open("/dev/gamepad", O_RDONLY);
  signal(SIGIO, &interrupt_handler);
  fcntl(descr, F_SETOWN, getpid());
  oflags = fcntl(descr, F_GETFL);
  fcntl(descr, F_SETFL, oflags | FASYNC);

}
