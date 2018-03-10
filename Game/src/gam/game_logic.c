#include "game_logic.h"

/***********************Global Variables**********************/
static enum state previousState = GAMEOVER;
static enum state currentState = MAIN;
static u8 buttons = 0;
static u8 num_bones = 0;

static u16 * frame = (u16 *)GAME_BASE_ADDR;
static struct sprite frisk;
static struct sprite platform_base;
static struct sprite platform_lvl1;
//not initialized yet
//static struct sprite platform_lvl2;
//static struct sprite platform_lvl3;
//static struct sprite papyrus;
//static struct sprite sans;
//static struct sprite bone;

struct image *frisk_still_img = (struct image *)FRISK_STILL_ADDR;
struct image *frisk_left_0_img = (struct image *)FRISK_LEFT_0_ADDR;
struct image *frisk_left_1_img = (struct image *)FRISK_LEFT_1_ADDR;
struct image *frisk_right_0_img = (struct image *)FRISK_RIGHT_0_ADDR;
struct image *frisk_right_1_img = (struct image *)FRISK_RIGHT_1_ADDR;
struct image *platform_base_img = (struct image *)PLATFORM_BASE_ADDR;
struct image *platform_lvl1_img = (struct image *)PLATFORM_LVL1_ADDR;

struct image * current_left;
struct image * current_right;

/***********************Prototypes*****************************/
static void ControllerCallback(void * data);
static void HandleButtons(void);
static void InitSprites(void);
static void DrawFrisk(void);
static void DrawBone(void);
static void Update(void);

static void DisplayMainMenu(void);
static void DisplayGameScene(void);
static void DisplayPauseMenu(void);
static void DisplayGameOver(void);

static void Move(enum direction dir);
static void Climb(enum direction dir);
static void Jump();
static void Grab();

static void CheckCollision();
static void CheckLeft();
static void CheckRight();
static void CheckBelow();


void StartGame(void)
{
	SetControllerCallback(ControllerCallback);

	InitSprites(); //initializes frisk sprite

	DisplayMainMenu(); //main menu background

	//game loop
	while (TRUE)
	{
#ifdef DEBUG
		if (currentState != previousState)
		{
			switch (currentState)
			{
				case MAIN:
				{
					xil_printf("=====Main Menu=====\r\nPress start to play...\r\n");
					break;
				}
				case PLAY:
				{
					if(previousState == MAIN)
						xil_printf("=====Game Started=====\r\n");
					else
						xil_printf("Continuing...\r\n");
					break;
				}
				case PAUSE:
				{
					xil_printf("Paused...\r\n");
					break;
				}
				case GAMEOVER:
				{
					xil_printf("=====Game Over=====\r\nPress Start to play again...\r\n");
					break;
				}
				default:
					xil_printf("Invalid State...\r\n");
					break;
			}

			previousState = currentState;
		}
#endif /*DEBUG*/

		HandleButtons();

		Update();

		CheckCollision(); //consists of check_left(), check_right(), check_below()

		DisplayGameScene();
		DrawImage(frame, platform_base.img, platform_base.pos.x, platform_base.pos.y);
		DrawFrisk();

		for (int i = 0; i < num_bones; i++)
		{
			DrawBone();
		}

		DrawFrame(frame);

		usleep(50000);
	}
}

void InitSprites(void)
{
	frisk.img = frisk_still_img;
	frisk.dim.width = 40;
	frisk.dim.height = 60;
	frisk.pos.x = 0;
	frisk.pos.y = 480 - frisk.dim.height;

	platform_base.img = platform_base_img;
	platform_base.dim.width = 640;
	platform_base.dim.height = 80;
	platform_base.pos.x = 0;
	platform_base.pos.y = 480 - platform_base.dim.height;

	platform_lvl1.img = 0;
	platform_lvl1.dim.width = 576;
	platform_lvl1.dim.height = 42;
	platform_lvl1.pos.x = (640 - platform_lvl1.dim.width)/2;
	platform_lvl1.pos.y = 480 - platform_base.pos.y - frisk.dim.height - platform_lvl1.dim.height;

	current_left = frisk_left_0_img;
	current_right = frisk_right_0_img;
}

void Update(void)
{
//	float time = (sleepTime / 1000000);
//	frisk_pos.x += xVelocity * time;
//	frisk_pos.y += yVelocity * time;
//
//	yVelocity -= GRAVITY * time;
//
//	if(y == level1 ||
//		y == level2 ||
//		y == level3 ||
//		y == level4 ||
//		y == level5 ||
//		y == level6)
//	{
//		//stop falling
//		yVelocity = 0;
//	}
}

void ControllerCallback(void * data)
{
	buttons = (u8) ((u32) data);
}

void HandleButtons(void)
{
	switch (currentState)
	{
		case MAIN:
		{
			if(buttons & START_MASK)
			{
				currentState = PLAY;
				DisplayGameScene();

				// wait until start is released
				while(buttons & START_MASK);
			}
			break;
		}
		case PLAY:
		{
			if (buttons & START_MASK)
			{
				currentState = PAUSE;
				DisplayPauseMenu();

				// wait until start is released
				while(buttons & START_MASK);
			}

			if(buttons & UP_MASK)
			{
				Climb(UP);
				xil_printf("Climbing up!\r\n");
			}
			else if (buttons & DOWN_MASK)
			{
				Climb(DOWN);
				xil_printf("Climbing down!\r\n");
			}
			else if (buttons & LEFT_MASK)
			{
				Move(LEFT);
				xil_printf("frisk(%d, %d)\r\n", frisk.pos.x, frisk.pos.y);
			}
			else if (buttons & RIGHT_MASK)
			{
				Move(RIGHT);
				xil_printf("frisk(%d, %d)\r\n", frisk.pos.x, frisk.pos.y);
			}

			if(buttons & A_MASK)
			{
				Jump();
				xil_printf("Jump!\r\n");
			}
			else if (buttons & B_MASK)
			{
				Grab();
				xil_printf("No power ups...\r\n");
			}
			break;
		}
		case PAUSE:
		{
			if (buttons & START_MASK)
			{
				currentState = PLAY;
				DisplayGameScene();

				// wait until start is released
				while(buttons & START_MASK);
			}
			break;
		}
		case GAMEOVER:
		{
			if (buttons & START_MASK)
			{
				currentState = MAIN;
				DisplayGameOver();

				// wait until start is released
				while(buttons & START_MASK);
			}
			break;
		}
		default:
			break;
	}
}

void DrawFrisk(void)
{
	DrawImage(frame, frisk.img, frisk.pos.x, frisk.pos.y);
}

void DrawBone(void)
{
	//DrawSprite(frame, bone, bone[0].x, bone[0].y);
}

static void DisplayMainMenu(void)
{
	struct image * menu_background;
	menu_background = (struct image *) MENU_BACKGROUND_ADDR;
	DrawImage(frame, menu_background, 0, 0);
}

static void DisplayGameScene(void)
{
	struct image * game_background;
	game_background = (struct image *) GAME_BACKGROUND_ADDR;
	DrawImage(frame, game_background, 0, 0);
}

static void DisplayPauseMenu(void)
{
//	struct image pauseMenu = (u16 *) PAUSE_MENU_LOCATION;
//	DrawSprite(frame, pauseMenu, middle.x, middle.y);
}

static void DisplayGameOver(void)
{
	struct image * gameover_background;
	gameover_background = (struct image *) GAMEOVER_BACKGROUND_ADDR;
	DrawImage(frame, gameover_background, 0, 0);
}

static void Move(enum direction dir)
{
	if (dir == LEFT)
#ifdef DEBUG
		if (frisk.pos.x == 0)
		{
			frisk.img = frisk_still_img;
		}

		else
		{
			frisk.img = current_left;
			frisk.pos.x -= HORIZONTAL_VELOCITY;

			if (current_left == frisk_left_0_img)
			{
				current_left = frisk_left_1_img;
			}

			else
			{
				current_left = frisk_left_0_img;
			}
		}

#else
		xVelocity = -HORIZONTAL_VELOCITY;
#endif
	else if (dir == RIGHT)
#ifdef DEBUG
		if (frisk.pos.x == (640 - 40))
		{
			frisk.img = frisk_still_img;
		}

		else
		{
			frisk.img = current_right;
			frisk.pos.x += HORIZONTAL_VELOCITY;

			if (current_right == frisk_right_0_img)
			{
				current_right = frisk_right_1_img;
			}

			else
			{
				current_right = frisk_right_0_img;
			}
		}

#else
		xVelocity = HORIZONTAL_VELOCITY;
#endif

	else
	{
		//do nothing
	}
}

static void Climb(enum direction dir)
{
//	if (dir == UP)
//	{
//		if(LadderNearby())
//		{
//			yVelocity = VERTICAL_VELOCITY;
//		}
//	}
//	else if (dir == UP)
//	{
//		if(LadderNearby())
//		{
//			yVelocity = -VERTICAL_VELOCITY;
//		}
//	}
}

static void Jump()
{
//	yVelocity = JUMP_VELOCITY;
}

static void Grab()
{
//	if (ItemNearby())
//	{
//		AddItem();
//	}
}

static void CheckCollision()
{
	CheckLeft();
	CheckRight();
	CheckBelow();
}
static void CheckLeft()
{
	//compare frisk.pos.x (the left side of frisk) with any mob or bonus sprites that may be on the left
	/*
	if (frisk.pos.x == (sprite.pos.x + sprite.dim.width))
	{

	}

	else if (frisk.pos.x == )
	{

	}*/
}
static void CheckRight()
{
	//compare (frisk.pos.x + frisk.dim.width) (the right side of frisk) with any mob or bonus sprites that may be on the right (their left)
	/*
	if ((frisk.pos.x + frisk.dim.width) == sprite.pos.x)
	{

	}

	else if ((frisk.pos.x frisk.dim.width) == )
	{

	}*/
}
static void CheckBelow()
{

}
