/*
 * game_main_menu.h
 *
 *  Created on: Mar 14, 2018
 *      Author: slippman
 */

#ifndef _GAME_MENUS_H_
#define _GAME_MENUS_H_

#include "../vga/axi_vga.h"
#include "../vid/video.h"
#include "../global.h"
#include "../tmr/timer.h"

#include "game_logic.h"

#define UPDATE_MS	(500)

void DisplaySplashScreen(u16 * gameFrame);
void DisplayMainMenu(void);
void UpdateMainMenu(void);
void DisplayPauseMenu(void);
void DisplayGameOver(void);
void UpdateGameOver(void);

#endif /* _GAME_MENU_H_ */
