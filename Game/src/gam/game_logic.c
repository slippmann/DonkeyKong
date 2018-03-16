#include "game_logic.h"
#include "game_menus.h"

/***********************Global Variables**********************/
#ifdef DEBUG
static enum state previousState = GAMEOVER;
#endif /*DEBUG*/

static enum state currentState = MAIN;
static u8 buttons = 0;
static bool pause_exit = false;

static u8 stepCount = 0;

static u16 * frame = (u16 *)GAME_BASE_ADDR;
static struct sprite frisk;

static struct sprite platform_base;
static struct sprite platform_lvl1;
static struct sprite platform_lvl2;
static struct sprite platform_lvl3;
static struct sprite platform_lvl4;

static struct sprite ladder_1;
static struct sprite ladder_2;
static struct sprite ladder_3;
static struct sprite ladder_4;

static struct sprite bonus_1;
static struct sprite bonus_2;

static struct sprite papyrus;
static struct sprite sans;

static struct sprite bones[MAX_NUM_BONES];

struct image *frisk_still_img = (struct image *)FRISK_STILL_ADDR;
struct image *frisk_left_0_img = (struct image *)FRISK_LEFT_0_ADDR;
struct image *frisk_left_1_img = (struct image *)FRISK_LEFT_1_ADDR;
struct image *frisk_right_0_img = (struct image *)FRISK_RIGHT_0_ADDR;
struct image *frisk_right_1_img = (struct image *)FRISK_RIGHT_1_ADDR;

struct image *platform_base_img = (struct image *)PLATFORM_BASE_ADDR;
struct image *platform_lvl_img = (struct image *)PLATFORM_LVL_ADDR;

struct image *ladder_img = (struct image *)LADDER_ADDR;
struct image *bonus_img = (struct image *)BONUS_ADDR;

struct image *papyrus_img;
struct image *sans_img;

struct image * current_left;
struct image * current_right;

/***********************Prototypes*****************************/
static void ControllerCallback(void * data);
static void HandleButtons(void);

static void InitSprites(void);
static void InitPlatforms(void);
static void InitLadders(void);
static void InitCharacters(void);
static void InitBonus();

static void DrawBone(void);
static void ResetGame(void);
static void Update(void);

static void DrawGameScene(void);

static void Move(enum direction dir);
static void Climb(enum direction dir);
static void Jump();
static void Grab();

//static void MobGeneration();
static void CheckCollision();
static bool AABBOverlap(struct sprite test);


void StartGame(void)
{
	SetControllerCallback(ControllerCallback);

	InitSprites(); //initializes frisk sprite

	DisplaySplashScreen(frame);

	DisplayMainMenu(); //main menu background

	//game loop
	while (true)
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
					{
						xil_printf("=====Game Started=====\r\n");
						DrawGameScene();
					}

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

		if(currentState == PLAY)
		{
			Update();

			CheckCollision();

			DrawGameScene();

			for (int i = 0; i < MAX_NUM_BONES; i++)
			{
				DrawBone();
			}

			UpdateScreen(frame);
		}

		usleep(20000);
	}
}

void InitSprites(void)
{
	InitPlatforms();
	InitLadders();
	InitCharacters();
	InitBonus();
	//instantiate bonus items
	//instantiate papyrus
	//instantiate sans
}

void InitPlatforms()
{
	platform_base.img = platform_base_img;
	platform_base.dim.width = 640;
	platform_base.dim.height = 32;
	platform_base.pos.x = 0;
	platform_base.pos.y = 480 - platform_base.dim.height;
	platform_base.type = PLATFORM;

	platform_lvl1.img = platform_lvl_img;
	platform_lvl1.dim.width = 576;
	platform_lvl1.dim.height = 16;
	platform_lvl1.pos.x = (640 - platform_lvl1.dim.width) / 2;
	platform_lvl1.pos.y = 480 - platform_base.dim.height - 1*(64 + platform_lvl1.dim.height);
	platform_lvl1.type = PLATFORM;

	platform_lvl2.img = platform_lvl_img;
	platform_lvl2.dim.width = 576;
	platform_lvl2.dim.height = 16;
	platform_lvl2.pos.x = (640 - platform_lvl2.dim.width) / 2;
	platform_lvl2.pos.y = 480 - platform_base.dim.height - 2*(64 + platform_lvl2.dim.height);
	platform_lvl2.type = PLATFORM;

	platform_lvl3.img = platform_lvl_img;
	platform_lvl3.dim.width = 576;
	platform_lvl3.dim.height = 16;
	platform_lvl3.pos.x = (640 - platform_lvl3.dim.width) / 2;
	platform_lvl3.pos.y = 480 - platform_base.dim.height - 3*(64 + platform_lvl3.dim.height);
	platform_lvl3.type = PLATFORM;

	platform_lvl4.img = platform_lvl_img;
	platform_lvl4.dim.width = 576;
	platform_lvl4.dim.height = 16;
	platform_lvl4.pos.x = (640 - platform_lvl4.dim.width) / 2;
	platform_lvl4.pos.y = 480 - platform_base.dim.height - 4*(64 + platform_lvl4.dim.height);
	platform_lvl4.type = PLATFORM;
}

void InitLadders()
{
	ladder_1.img = ladder_img;
	ladder_1.dim.width = 24;
	ladder_1.dim.height = 64;
	ladder_1.pos.x = 640 - 100;
	ladder_1.pos.y = platform_lvl1.pos.y + platform_lvl1.dim.height - 4;
	ladder_1.type = LADDER;

	ladder_2.img = ladder_img;
	ladder_2.dim.width = 24;
	ladder_2.dim.height = 64;
	ladder_2.pos.x = 0 + 100;
	ladder_2.pos.y = platform_lvl2.pos.y + platform_lvl2.dim.height;
	ladder_2.type = LADDER;

	ladder_3.img = ladder_img;
	ladder_3.dim.width = 24;
	ladder_3.dim.height = 64;
	ladder_3.pos.x = 640 - 100;
	ladder_3.pos.y = platform_lvl3.pos.y + platform_lvl3.dim.height;
	ladder_3.type = LADDER;

	ladder_4.img = ladder_img;
	ladder_4.dim.width = 24;
	ladder_4.dim.height = 64;
	ladder_4.pos.x = 0 + 100;
	ladder_4.pos.y = platform_lvl4.pos.y + platform_lvl4.dim.height;
	ladder_4.type = LADDER;
}

void InitCharacters()
{
	frisk.img = frisk_still_img;
	frisk.dim.width = 22;
	frisk.dim.height = 32;
	frisk.pos.x = 0;
	frisk.pos.y = 480 - platform_base.dim.height - frisk.dim.height;
	frisk.type = CHAR;
	current_left = frisk_left_0_img;
	current_right = frisk_right_0_img;

	sans.img;
	sans.dim.width;
	sans.dim.height;
	sans.pos.x;
	sans.pos.y;
	sans.type = CHAR;

	papyrus.img;
	papyrus.dim.width;
	papyrus.dim.height;
	papyrus.pos.x;
	papyrus.pos.y;
	papyrus.type = CHAR;
}

void InitBonus()
{
	bonus_1.img = bonus_img;
	bonus_1.dim.width = 18;
	bonus_1.dim.height = 12;
	bonus_1.pos.x = platform_lvl1.pos.x + bonus_1.dim.width;
	bonus_1.pos.y = platform_lvl1.pos.y - bonus_1.dim.height;
	bonus_1.type = BONUS;

	bonus_2.img = bonus_img;
	bonus_2.dim.width = 18;
	bonus_2.dim.height = 12;
	bonus_2.pos.x = platform_lvl3.pos.x + platform_lvl3.dim.width - 2*(bonus_2.dim.width);
	bonus_2.pos.y = platform_lvl3.pos.y - bonus_2.dim.height;
	bonus_2.type = BONUS;
}

void ResetGame(void)
{
	InitSprites();

	DrawGameScene();
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
			UpdateMainMenu();

			if(buttons & START_MASK)
			{
				ResetGame();

				currentState = PLAY;

				// wait until start is released
				while(buttons & START_MASK);

				DrawGameScene();
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

				// wait until start is released
				while(buttons & START_MASK);

				DrawGameScene();
			}
			else if (buttons & A_MASK)
			{
				// wait until start is released
				while(buttons & A_MASK);

				if(pause_exit == false)
				{
					currentState = PLAY;
					DrawGameScene();
				}
				else
				{
					//TODO
//					currentState = MAIN;
//					DisplayMainMenu();

					currentState = GAMEOVER;
					DisplayGameOver();
				}
			}
			else if(buttons & RIGHT_MASK || buttons & LEFT_MASK)
			{
				struct image * pauseMenu = (struct image *) PAUSE_MENU_BACKGROUND;
				struct image * button;

				if(buttons & RIGHT_MASK)
				{
					DrawImage(frame, pauseMenu, 194, 90); // centered at (195, 90)

					button = (struct image *) PAUSE_MENU_EXIT;
					DrawImage(frame, button, (194 + 140), (90 + 244)); // exit at (140, 244)

					UpdateScreen(frame);

					pause_exit = true;
				}
				else if (buttons & LEFT_MASK)
				{
					DrawImage(frame, pauseMenu, 194, 90); // centered at (195, 90)

					button = (struct image *) PAUSE_MENU_CONTINUE;
					DrawImage(frame, button, (194 + 30), (90 + 244)); // continue at (30, 244)

					UpdateScreen(frame);

					pause_exit = false;
				}
			}

			break;
		}
		case GAMEOVER:
		{
			UpdateGameOver();

			if (buttons & START_MASK)
			{
				currentState = MAIN;
				DisplayMainMenu();

				// wait until start is released
				while(buttons & START_MASK);
			}
			break;
		}
		default:
			break;
	}
}

void DrawBone(void)
{
	//DrawSprite(frame, bone, bone[0].x, bone[0].y);
}

void DrawGameScene(void)
{
	//set up background
	struct image * game_background;
	game_background = (struct image *) GAME_BACKGROUND;
	DrawImage(frame, game_background, 0, 0);

	//set up platforms
	DrawImage(frame, platform_base.img, platform_base.pos.x, platform_base.pos.y);
	DrawImage(frame, platform_lvl1.img, platform_lvl1.pos.x, platform_lvl1.pos.y);
	DrawImage(frame, platform_lvl2.img, platform_lvl2.pos.x, platform_lvl2.pos.y);
	DrawImage(frame, platform_lvl3.img, platform_lvl3.pos.x, platform_lvl3.pos.y);
	DrawImage(frame, platform_lvl4.img, platform_lvl4.pos.x, platform_lvl4.pos.y);

	//set up ladders
	DrawImage(frame, ladder_1.img, ladder_1.pos.x, ladder_1.pos.y);
	DrawImage(frame, ladder_2.img, ladder_2.pos.x, ladder_2.pos.y);
	DrawImage(frame, ladder_3.img, ladder_3.pos.x, ladder_3.pos.y);
	DrawImage(frame, ladder_4.img, ladder_4.pos.x, ladder_4.pos.y);

	//set up bonus items
	DrawImage(frame, bonus_1.img, bonus_1.pos.x, bonus_1.pos.y);
	DrawImage(frame, bonus_2.img, bonus_2.pos.x, bonus_2.pos.y);

	//set up frisk
	DrawImage(frame, frisk.img, frisk.pos.x, frisk.pos.y);
}

static void Move(enum direction dir)
{
	if (dir == LEFT)
#ifdef DEBUG
		if (frisk.pos.x <= 0)
		{
			frisk.pos.x = 0;
			frisk.img = frisk_still_img;
		}

		else
		{
			frisk.img = current_left;
			frisk.pos.x -= HORIZONTAL_VELOCITY;

			stepCount++;

			if(stepCount == 2)
			{
				current_left =  current_left == frisk_left_0_img ? frisk_left_1_img : frisk_left_0_img;
				stepCount = 0;
			}
		}

#else
		xVelocity = -HORIZONTAL_VELOCITY;
#endif
	else if (dir == RIGHT)
#ifdef DEBUG
		if (frisk.pos.x >= (640 - frisk.dim.width))
		{
			frisk.pos.x = 640 - frisk.dim.width;
			frisk.img = frisk_still_img;
		}

		else
		{
			frisk.img = current_right;
			frisk.pos.x += HORIZONTAL_VELOCITY;

			stepCount++;

			if (stepCount == 2)
			{
				current_right = current_right == frisk_right_0_img ? frisk_right_1_img : frisk_right_0_img;
				stepCount = 0;
			}
		}

#else
		xVelocity = HORIZONTAL_VELOCITY;
#endif

	else if (dir == UP)//check ladder first
	{
		frisk.pos.y += -VERTICAL_VELOCITY;

	}

	else if (dir == DOWN)
	{
		frisk.pos.y += VERTICAL_VELOCITY;
	}

	else
	{
		//do nothing
	}
}

static void Climb(enum direction dir)
{
	if (dir == UP)
	{
		if(AABBOverlap(ladder_1))
		{
			if (frisk.pos.y + frisk.dim.height > platform_lvl1.pos.y)
			{
				frisk.pos.x = ladder_1.pos.x + 2;
				Move(UP);
			}

			else if (frisk.pos.y + frisk.dim.height <= platform_lvl1.pos.y)
			{
				frisk.pos.y = platform_lvl1.pos.y - frisk.dim.height;
			}

			else
			{
				//do nothing
			}
		}

		else if (AABBOverlap(ladder_2))
		{
			if (frisk.pos.y + frisk.dim.height > platform_lvl2.pos.y)
			{
				frisk.pos.x = ladder_2.pos.x + 2;
				Move(UP);
			}

			else if (frisk.pos.y + frisk.dim.height <= platform_lvl2.pos.y)
			{
				frisk.pos.y = platform_lvl2.pos.y - frisk.dim.height;
			}

			else
			{
				//do nothing
			}
		}

		else if (AABBOverlap(ladder_3))
		{
			if (frisk.pos.y + frisk.dim.height > platform_lvl3.pos.y)
			{
				frisk.pos.x = ladder_3.pos.x + 2;
				Move(UP);
			}

			else if (frisk.pos.y + frisk.dim.height <= platform_lvl3.pos.y)
			{
				frisk.pos.y = platform_lvl3.pos.y - frisk.dim.height;
			}

			else
			{
				//do nothing
			}
		}

		else if (AABBOverlap(ladder_4))
		{
			if (frisk.pos.y + frisk.dim.height > platform_lvl4.pos.y)
			{
				frisk.pos.x = ladder_4.pos.x + 2;
				Move(UP);
			}

			else if (frisk.pos.y + frisk.dim.height <= platform_lvl4.pos.y)
			{
				frisk.pos.y = platform_lvl4.pos.y - frisk.dim.height;
			}

			else
			{
				//do nothing
			}
		}

		else
		{
			//do nothing
		}
	}

	else if (dir == DOWN)
	{
		//checks if player is above ladder_1
		if(AABBOverlap(ladder_1) && (frisk.pos.x >= ladder_1.pos.x && (frisk.pos.x+frisk.dim.width) <= (ladder_1.pos.x+ladder_1.dim.width)))
		{
			if (frisk.pos.y + frisk.dim.height < platform_base.pos.y)
			{
				Move(DOWN);
			}

			else if (frisk.pos.y + frisk.dim.height >= platform_base.pos.y)
			{
				frisk.pos.y = platform_base.pos.y - frisk.dim.height;
			}

			else
			{
				//do nothing
			}
		}

		//checks if player is above ladder_2
		else if (AABBOverlap(ladder_2) && (frisk.pos.x >= ladder_2.pos.x && (frisk.pos.x+frisk.dim.width) <= (ladder_2.pos.x+ladder_2.dim.width)))
		{
			if (frisk.pos.y + frisk.dim.height < platform_lvl1.pos.y)
			{
				Move(DOWN);
			}

			else if (frisk.pos.y + frisk.dim.height >= platform_lvl1.pos.y)
			{
				frisk.pos.y = platform_lvl1.pos.y - frisk.dim.height;
			}

			else
			{
				//do nothing
			}
		}

		//checks if player is above ladder_3
		else if (AABBOverlap(ladder_3) && (frisk.pos.x >= ladder_3.pos.x && (frisk.pos.x+frisk.dim.width) <= (ladder_3.pos.x+ladder_3.dim.width)))
		{
			if (frisk.pos.y + frisk.dim.height < platform_lvl2.pos.y)
			{
				Move(DOWN);
			}

			else if (frisk.pos.y + frisk.dim.height >= platform_lvl2.pos.y)
			{
				frisk.pos.y = platform_lvl2.pos.y - frisk.dim.height;
			}

			else
			{
				//do nothing
			}
		}

		//checks if player is above ladder_4
		else if (AABBOverlap(ladder_4) && (frisk.pos.x >= ladder_4.pos.x && (frisk.pos.x+frisk.dim.width) <= (ladder_4.pos.x+ladder_4.dim.width)))
		{
			if (frisk.pos.y + frisk.dim.height < platform_lvl3.pos.y)
			{
				Move(DOWN);
			}

			else if (frisk.pos.y + frisk.dim.height >= platform_lvl3.pos.y)
			{
				frisk.pos.y = platform_lvl3.pos.y - frisk.dim.height;
			}

			else
			{
				//do nothing
			}
		}

		//the player is not above a ladder
		else
		{
			//do nothing
		}
	}
}

static void Jump()
{
	//dont forget to check if there is a sprite beneath you
}

static void Grab()
{
	if (AABBOverlap(bonus_1))
	{
		//remove bonus_1
		//add 500 points to point total
	}

	else if (AABBOverlap(bonus_2))
	{
		//remove bonus_2
		//add 500 points to point total
	}

	else
	{
		//do nothing
	}
}

static void CheckCollision()
{
	u16 size = sizeof(bones) / sizeof(bones[0]);
	for(u16 i = 0; i < size; i++)
	{
		if (AABBOverlap(bones[i]))
		{
			DisplayGameOver();
		}

		else
		{
			//do nothing
		}
	}
}

static bool AABBOverlap(struct sprite test)
{
	short d1x = frisk.pos.x - (test.dim.width + test.pos.x);
	short d1y = frisk.pos.y - (test.dim.height + test.pos.y);
	short d2x = test.pos.x - (frisk.dim.width + frisk.pos.x);
	short d2y = test.pos.y - (frisk.dim.height + frisk.pos.y);

	if (d1x > 0 || d1y > 0)
		return false;

	if (d2x > 0 || d2y > 0)
		return false;

	return true;
}
