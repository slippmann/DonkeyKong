#include "game_logic.h"

/***********************Global Variables**********************/
static enum state previousState = GAMEOVER;
static enum state currentState = MAIN;
static u8 buttons = 0;
static u8 num_bones = 0;

static u16 * frame = (u16 *)GAME_BASE_ADDR;
static struct sprite frisk;

/***********************Prototypes*****************************/
static void ControllerCallback(void * data);
static void HandleButtons(void);
static void InitSprites(void);
static void DrawFrisk(void);
static void DrawBone(void);
static void Update(void);

static void DisplayMainMenu(void);
static void DisplayScene(void);
static void DisplayPauseMenu(void);
static void DisplayGameOver(void);

static void Move(enum direction dir);
static void Climb(enum direction dir);
static void Jump();
static void Grab();


void StartGame(void)
{
	SetControllerCallback(ControllerCallback);

	InitSprites();

	DisplayMainMenu();

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

		DrawFrisk();

		for (int i = 0; i < num_bones; i++)
		{
			DrawBone();
		}


	}
}

void InitSprites(void)
{
	frisk.img = (struct image *) CONTENT_BASE_ADDR;
	frisk.pos.x = 0;
	frisk.pos.y = 0;
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
				DisplayScene();
			}
			break;
		}
		case PLAY:
		{
			if (buttons & START_MASK)
			{
				currentState = PAUSE;
				DisplayPauseMenu();
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
				DisplayScene();
			}
			break;
		}
		case GAMEOVER:
		{
			if (buttons & START_MASK)
			{
				currentState = MAIN;
				DisplayMainMenu();
			}
			break;
		}
		default:
			break;
	}
}

void DrawFrisk(void)
{
	//DrawSprite(frame, frisk, frisk_pos.x, frisk_pos.y);
}

void DrawBone(void)
{
	//DrawSprite(frame, bone, bone[0].x, bone[0].y);
}

static void DisplayMainMenu(void)
{
//	frame = (u16 *) MAIN_MENU_LOCATION;
}

static void DisplayScene(void)
{
//	frame = (u16 *) BACKGROUND_LOCATION;
}

static void DisplayPauseMenu(void)
{
//	struct image pauseMenu = (u16 *) PAUSE_MENU_LOCATION;
//	DrawSprite(frame, pauseMenu, middle.x, middle.y);
}

static void DisplayGameOver(void)
{
//	frame = (u16 *) MAIN_MENU_LOCATION;
}

static void Move(enum direction dir)
{
	if (dir == LEFT)
#ifdef DEBUG
		frisk.pos.x -= 5;
#else
		xVelocity = -HORIZONTAL_VELOCITY;
#endif
	else if (dir == RIGHT)
#ifdef DEBUG
		frisk.pos.x += 5;
#else
		xVelocity = HORIZONTAL_VELOCITY;
#endif
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
