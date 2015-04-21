/*
 * game.h
 *
 *  Created on: 19. apr. 2015
 *      Author: jonzk_000
 */

#ifndef GAME_H_
#define GAME_H_

#include <stdint.h>
#include <stdbool.h>

#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"

#include "bspconfig.h"

#include "display.h"
#define DISPLAY_DEVICE_NO	(0)

#include "graphics.h"

/* Masks */
#define LEFT 			-1
#define RIGHT 			1
#define UP				-1
#define DOWN			1
#define BAR_MOVE		1

/* System constants */
#define SCREEN_RES_Y 	240		// Pixels
#define SCREEN_RES_X 	320		// Pixels
#define FPS				20

void GAME_init(void);
void GAME_loop(void);
void GAME_timerEventHandler(void);

#endif /* GAME_H_ */
