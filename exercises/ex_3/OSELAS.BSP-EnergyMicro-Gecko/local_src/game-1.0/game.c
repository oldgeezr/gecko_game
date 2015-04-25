#ifndef _GNU_SOURCE
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
uint16_t xNow, yNow, barNow;

void gpioSetup();
void interrupt_handler(int signo);
void moveBar(int16_t dir);
void moveBall(int8_t xTrajectory, int8_t yDir, uint16_t lvl);
bool ballMissBar(int dir);

static inline void GAME_timerEventHandler(void) {

	if (!gameInitiated) {
		return;
	}

	state = nextState;
	gameUpdateFlag = true;
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

	while(1) {
		if (gameUpdateFlag) {
			gameUpdateFlag = false;
			switch(state) {
				case welcomeScreen:
					GRAPHICS_showWelcomeScreen();
					nextState = welcomeScreenWait;
					break;
				case welcomeScreenWait:
					if (pb0Pressed || pb1Pressed) {
						nextState = gameInit;
					}
					break;
				case gameInit:
					xNow = SCREEN_RES_X/2;
					yNow = 150;
					barNow = SCREEN_RES_X/2;

					ballDir = DOWN;
					path = 1;
					bounces = 0;
					level = 1;

					GRAPHICS_clearScreen();
					GRAPHICS_printLevel(level);
					GRAPHICS_printScore(bounces);
					GRAPHICS_printBackground();
					GRAPHICS_printBar(barNow);
					GRAPHICS_printBall(xNow, yNow);

					nextState = gameRunning;
					break;
				case gameRunning:
					if(!ballMissBar(ballDir)) {
						// Handle buttons
						if(pb0Pressed && !pb1Pressed) {
							moveBar(LEFT);
						}
						if(pb1Pressed && !pb0Pressed) {
							moveBar(RIGHT);
						}
						// When ball hits ceiling
						if(yNow - BALL_RADIUS - 20 <= 0) {
							ballDir = DOWN;
							path = prevPath;
							prevPath = path;
						}
						// When ball hits left wall
						else if(xNow - BALL_RADIUS - 5 <= 0) {
							path = (-1) * prevPath;
							prevPath = path;
						}
						// When ball hits right wall
						else if(xNow + BALL_RADIUS + 5 >= SCREEN_RES_X) {
							path = (-1) * prevPath;
							prevPath = path;
							moveBall(path, ballDir, level);
						}
						// When ball hits bar
						else if((yNow + BALL_RADIUS + 5 >= SCREEN_RES_Y - BAR_HEIGHT) && (xNow >= barNow ) && (xNow <= barNow + BAR_LENGTH) ){
							GRAPHICS_printScore(++bounces);
							percentage = ((double)(2*(xNow - (barNow + BAR_LENGTH/2))))/((double)BAR_LENGTH);
							path = (int)(percentage * (double)X_MAX);
							prevPath = path;
							ballDir = UP;
						}
						moveBall(path, ballDir, level);

						if(bounces > 10) {
							GRAPHICS_printLevel(++level);
							level++;
							bounces = 0;
						}
					} else {
						nextState = gameOver;
					}
					break;
				case gameOver:
					GRAPHICS_gameOver();
					nextState = gameOverWait;
					break;
				case gameOverWait:
					sleep(5);
					nextState = welcomeScreen;
					break;
			}
		}
		else {
			usleep(50000);
			GAME_timerEventHandler();
		}
	}
}

void moveBar(int16_t dir) {

	if(dir < 0 && (barNow > 0)) {
		GRAPHICS_clearBar(barNow);
		barNow = barNow - BAR_MOVE;
		GRAPHICS_printBar(barNow);
	}
	else if(dir > 0 && (barNow < (SCREEN_RES_X - BAR_LENGTH))) {
		GRAPHICS_clearBar(barNow);
		barNow = barNow + BAR_MOVE;
		GRAPHICS_printBar(barNow);
	}
}

bool ballMissBar(int dir) {

	if((yNow + BALL_RADIUS + BAR_HEIGHT) > SCREEN_RES_Y && dir > 0){
		return true;
	} else {
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

void moveBall(int8_t xTrajectory, int8_t yDir, uint16_t lvl) {

	GRAPHICS_clearBall(xNow, yNow);
	xNow = xNow + xTrajectory;
	if(yDir < 0) {
		yNow = yNow - (BALL_MOVE+lvl);
	} else {
		yNow = yNow + (BALL_MOVE+lvl);
	}
	GRAPHICS_printBall(xNow, yNow);
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
}
