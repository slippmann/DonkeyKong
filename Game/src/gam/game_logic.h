/*
 * game_logic.h
 *
 *  Created on: Mar 7, 2018
 *      Author: slippman
 */

#ifndef _GAME_LOGIC_H_
#define _GAME_LOGIC_H_

#include "../global.h"
#include "../ctr/axi_controller.h"
#include "../vga/axi_vga.h"
#include "../vid/video.h"

#define UP_MASK		(0x01)
#define DOWN_MASK	(0x02)
#define LEFT_MASK	(0x04)
#define RIGHT_MASK	(0x08)
#define START_MASK	(0x10)
#define A_MASK		(0x20)
#define B_MASK		(0x40)

#define MAX_NUM_BONES   		(5)

#define HORIZONTAL_VELOCITY 	(6)
#define VERTICAL_VELOCITY 		(5)
#define JUMP_VELOCITY 			(10)
#define GRAVITY 				(-2)

enum direction
{
	UP, DOWN, LEFT, RIGHT
};

struct position
{
	u16 x;
	u16 y;
};

struct sprite
{
	struct image *img;
	struct position pos;
};

enum state
{
	MAIN, PLAY, PAUSE, GAMEOVER
};

void StartGame(void);

#endif /* _GAME_LOGIC_H_ */
