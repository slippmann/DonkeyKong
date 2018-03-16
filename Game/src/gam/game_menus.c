#include "game_menus.h"

static u16 * frame;
static volatile bool updatePending = false;
static bool isDrawing = false;

static void TimerUpdate();

/*********************************
 *
 * DisplaySplashScreen
 *
 * Displays the game splash screen and initializes the gameFrame
 *
 * params:	gameFrame - the games frame location
 *
 * return:	None
 *
 *********************************/
void DisplaySplashScreen(u16 * gameFrame)
{
	frame = gameFrame;

	struct image * splash_screen;
	splash_screen = (struct image *) SPLASH_SCREEN;
	DrawImage(frame, splash_screen, 0, 0);

	UpdateScreen(frame);

	sleep(4); // show splash screen for 4 seconds
}

/*********************************
 *
 * DisplayMainMenu
 *
 * Displays the game main menu
 *
 * params:	None
 *
 * return:	None
 *
 *********************************/
void DisplayMainMenu(void)
{
	struct image * menubackground;
	struct image * pressstart;
	menubackground = (struct image *) MAIN_MENU_BACKGROUND;
	pressstart = (struct image *) MAIN_MENU_PRESS_START;
	DrawImage(frame, menubackground, 0, 0);
	DrawImage(frame, pressstart, 240, 250); // text at (240, 250)

	UpdateScreen(frame);

	isDrawing = false;

	StartTimer(UPDATE_MS, false, TimerUpdate);
}

/*********************************
 *
 * DisplaySplashScreen
 *
 * Displays the in game pause menu
 *
 * params:	None.
 *
 * return:	None.
 *
 *********************************/
void DisplayPauseMenu(void)
{
	struct image * pauseMenu = (struct image *) PAUSE_MENU_BACKGROUND;
	struct image * cont = (struct image *) PAUSE_MENU_CONTINUE;
	DrawImage(frame, pauseMenu, 194, 90); // centered at (195, 90)

	DrawImage(frame, cont, (194 + 30), (90 + 244)); // continue at (30, 244)

	UpdateScreen(frame);
}

/*********************************
 *
 * DisplaySplashScreen
 *
 * Displays the game over screen
 *
 * params:	None
 *
 * return:	None
 *
 *********************************/
void DisplayGameOver(void)
{
	struct image * gameover_background;
	struct image * dontgiveup;
	struct image * staydetermined;
	struct image * pressstart;
	gameover_background = (struct image *) GAME_OVER_BACKGROUND;
	dontgiveup = (struct image *) GAME_OVER_DONT_GIVE_UP;
	staydetermined = (struct image *) GAME_OVER_STAY_DETERMINED;
	pressstart = (struct image *) GAME_OVER_PRESS_START;
	DrawImage(frame, gameover_background, 0, 0);
	DrawImage(frame, dontgiveup, 172, 326); // text at (172, 326)

	UpdateScreen(frame);
	sleep(3);

	DrawImage(frame, staydetermined, 172, 326); // text at (172, 326)

	UpdateScreen(frame);
	sleep(3);

	DrawImage(frame, pressstart, 172, 326); // text at (172, 326)

	UpdateScreen(frame);

	isDrawing = false;

	StartTimer(UPDATE_MS, false, TimerUpdate);
}

void TimerUpdate()
{
	updatePending = true;
}

void UpdateMainMenu()
{
	struct image * menubackground;
	struct image * pressstart;
	menubackground = (struct image *) MAIN_MENU_BACKGROUND;
	pressstart = (struct image *) MAIN_MENU_PRESS_START;

	if (updatePending)
	{
		updatePending = false;

		if (isDrawing)
			DrawImage(frame, pressstart, 240, 250); // text at (240, 250)
		else
			EraseImage(frame, menubackground, pressstart, 240, 250);

		isDrawing = !isDrawing;

		UpdateScreen(frame);

		StartTimer(UPDATE_MS, false, TimerUpdate);
	}

}

void UpdateGameOver()
{
	struct image * staydetermined;
	struct image * pressstart;
	staydetermined = (struct image *) GAME_OVER_STAY_DETERMINED;
	pressstart = (struct image *) GAME_OVER_PRESS_START;

	if (updatePending)
	{
		updatePending = false;

		if (isDrawing)
			DrawImage(frame, pressstart, 172, 326); // text at (172, 326)
		else
			DrawImage(frame, staydetermined, 172, 326); // text at (172, 326)

		isDrawing = !isDrawing;

		UpdateScreen(frame);

		StartTimer(UPDATE_MS, false, TimerUpdate);
	}
}
