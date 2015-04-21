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

#include "graphics.h"
#include "game.h"
#include "display.h"

#define true 1
#define false 0

#define SW0 0
#define SW1 1
#define SW7 64

typedef char bool;

int descr;
int buff;

typedef enum _gameState {
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
uint16_t xNow, yNow, barNow;

bool ballMissBar(int dir);
void gpioSetup();
void interrupt_handler(int signo);
void moveBar(int16_t dir);
void moveBall(int8_t xTrajectory, int8_t yDir, uint16_t lvl);
bool ballMissBar(int dir);

static inline void GAME_timerEventHandler(void) {

  static int frameUpdateCount    = 0;
  //static int capsenseUpdateCount = 0;

  /* Return at once if init has not been called */
  if (!gameInitiated) {
    return;
  }

  //if (++frameUpdateCount == FPS) {

    frameUpdateCount = 0;
    /* Update state */
    state = nextState;
    /* Set update flag for game */
    gameUpdateFlag = true;
  //}

  /* Update wait timer */
  if (timeout > 0) {
    timeout--;
  }

  return;
}

void GAME_init(void) {

  display_setup();
  printf("Display setup - OK\n");
  gpioSetup();
  printf("GPIO setup - OK\n");
  state = welcomeScreen;
  nextState = state;
  gameInitiated = true;
}

void GAME_loop(void) {

  uint16_t level, bounces;
  int ballDir, path, prevPath;
  double percentage;

  level = 0;
  bounces = 0;
  prevPath = 0;
  path = 0;
  ballDir = 0;

  //gameUpdateFlag = 1;

  while(1) {
    if (gameUpdateFlag) {

      gameUpdateFlag = false;
      switch(state) {

        case welcomeScreen:
          GRAPHICS_showWelcomeScreen();
          nextState = welcomeScreenWait;
          printf("STATE: Welcome Screen\n");
          break;

        case welcomeScreenWait:
          /* Start the game when PB0 or PB1 is pressed */
          if (pb0Pressed || pb1Pressed) {
            nextState = gameInit;
          }
          printf("STATE: Welcome screen wait\n");
          break;

        case gameInit:
          xNow = SCREEN_RES_X/2;
          yNow = 150;
          barNow = SCREEN_RES_X/2;

          GRAPHICS_clearScreen();
          GRAPHICS_printBar(barNow);
          GRAPHICS_printBall(xNow, yNow);

          ballDir = DOWN;
          path = 3;
          bounces = 0;
          level = 1;

          nextState = gameRunning;

          printf("STATE: Game Init\n");
          break;

        case gameRunning:
          /* Check loose-constraints */
          if(!ballMissBar(ballDir)) {

            /* Handle bar movement for button presses */
            if(pb0Pressed && !pb1Pressed) {
              moveBar(LEFT);
              //printf("STATE: Game -> Moving Left\n");
            }
            if(pb1Pressed && !pb0Pressed) {
              moveBar(RIGHT);
              //printf("STATE: Game -> Moving right\n");
            }

            /* Handle ball movement */
            // When ball hits ceiling
            if(yNow - BALL_RADIUS <= 0) {
              ballDir = DOWN;
              path = prevPath;
              prevPath = path;
              //printf("STATE: Game -> Ball hit ceiling\n");
            }
            // When ball hits left wall
            else if(xNow - BALL_RADIUS <= 0) {
              path = (-1) * prevPath;
              prevPath = path;
              //printf("STATE: Game -> Ball hit left wall\n");
            }
            // When ball hits right wall
            else if(xNow + BALL_RADIUS >= SCREEN_RES_X) {
              path = (-1) * prevPath;
              prevPath = path;
              moveBall(path, ballDir, level);
              printf("STATE: Game -> Ball hit right wall\n");
            }
            // When ball hits bar
            else if((yNow + BALL_RADIUS >= SCREEN_RES_Y - BAR_HEIGHT - 5) && (xNow >= barNow ) && (xNow <= barNow + BAR_LENGTH) ){
              // A bit sketchy
              //double temp = (cos((((xNow - (barNow + (BAR_LENGTH/2)))/(BAR_LENGTH/2)) * 75)));
              percentage = ((double)(2*(xNow - (barNow + BAR_LENGTH/2))))/((double)BAR_LENGTH);
              path = (int)(percentage * (double)X_MAX);
              printf("STATE:Game, path = %d - %f \n",path,percentage);
              //path = 1;
              prevPath = path;
              ballDir = UP;
              bounces++;
              printf("STATE: Game -> Ball hits bar\n");
            }
            // Moving freely
            moveBall(path, ballDir, level);

            if(bounces > 10) {
              level++;
              bounces = 0;
            }
          }
          else {
            nextState = gameOver;
          }
          //printf("STATE: Game\n");

          break;

        case gameOver:
          // Clear screen and finalize
          nextState = gameOverWait;
          printf("STATE: Game Over\n");
          break;
        case gameOverWait:
          // Wait
          nextState = welcomeScreen;
          printf("STATE: Game Over Wait");
          break;
      }
    }
    else {
      //usleep(1000);
      display_refresh();
      GAME_timerEventHandler();
    }
  }
}

void moveBar(int16_t dir) {
  if(dir < 0 && (barNow > 0)){
    GRAPHICS_clearBar(barNow);
    barNow = barNow - BAR_MOVE;
    GRAPHICS_printBar(barNow);
  }
  else if(dir > 0 && (barNow < (SCREEN_RES_X - BAR_LENGTH))){
    GRAPHICS_clearBar(barNow);
    barNow = barNow + BAR_MOVE;
    GRAPHICS_printBar(barNow);
  }
  //display_refresh();
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

void gpioSetup(void) {

  int oflags;
  descr = open("/dev/gamepad", O_RDONLY);
  signal(SIGIO, &interrupt_handler);
  fcntl(descr, F_SETOWN, getpid());
  oflags = fcntl(descr, F_GETFL);
  fcntl(descr, F_SETFL, oflags | FASYNC);

}

// Antar 1 pixelbevegelse i Y-akse, 0 er rett opp. Speed 0 er stopp, 1 er laveste hastighet, osv.
void moveBall(int8_t xTrajectory, int8_t yDir, uint16_t lvl) {
  GRAPHICS_clearBall(xNow, yNow);
  xNow = xNow + xTrajectory;
  if(yDir < 0) {
    yNow = yNow - BALL_MOVE*lvl;
  } else {
    yNow = yNow + BALL_MOVE*lvl;
  }
  GRAPHICS_printBall(xNow, yNow);

  //display_refresh();
}

int main(void) {

  GAME_init();
  printf("Game Init - OK\n");
  GAME_loop();
  printf("Help! I'm out of the loop :( \n");

  return 0;
}

void interrupt_handler(int signo) {

  read((descr), &buff, 1);
  //printf("type: %d reading %d\n", signo, buff);

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

  //printf("pb0 %d, pb1 %d\n", pb0Pressed, pb1Pressed);
}


