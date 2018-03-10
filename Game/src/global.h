#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "xparameters.h"
#include "xil_exception.h"
#include "xdebug.h"
#include "string.h"
#include "sleep.h"

#define DEBUG

#define DDR_BASE_ADDR			XPAR_PS7_DDR_0_S_AXI_BASEADDR
#define MEM_BASE_ADDR		  	(DDR_BASE_ADDR + 0x1000000)

#define FRAME_WIDTH 			(640)
#define FRAME_HEIGHT			(480)
#define FRAME_LEN				(FRAME_WIDTH * FRAME_HEIGHT * 2) /* 640x480 = 307200 pixels * 2 bytes/pixel= 614400 bytes */

#define VIDEO_MEM_SIZE		 	(FRAME_LEN * 2) /* two frames */

#define GAME_BASE_ADDR			(MEM_BASE_ADDR + VIDEO_MEM_SIZE)

#define IMG_HEADER_LEN			(4)
#define CONTENT_BASE_ADDR		(DDR_BASE_ADDR + 0x2000000)

//image and song file locations
#define GAME_BACKGROUND_ADDR 		(CONTENT_BASE_ADDR)	//0x0210 0000
#define MENU_BACKGROUND_ADDR		(GAME_BACKGROUND_ADDR + IMG_HEADER_LEN + FRAME_LEN)//0x0219 6004
#define GAMEOVER_BACKGROUND_ADDR	(MENU_BACKGROUND_ADDR + IMG_HEADER_LEN + FRAME_LEN)//0x0222 C008
#define FRISK_STILL_ADDR			(GAMEOVER_BACKGROUND_ADDR + IMG_HEADER_LEN + FRAME_LEN)//0x022C 200C
#define FRISK_LEFT_0_ADDR			(FRISK_STILL_ADDR + IMG_HEADER_LEN + 0x1200)//0x022C 3210
#define FRISK_LEFT_1_ADDR			(FRISK_LEFT_0_ADDR + IMG_HEADER_LEN + 0x1200)//0x022C 4414
#define FRISK_RIGHT_0_ADDR			(FRISK_LEFT_1_ADDR + IMG_HEADER_LEN + 0X1200)//0X022C 5618
#define FRISK_RIGHT_1_ADDR			(FRISK_RIGHT_0_ADDR + IMG_HEADER_LEN + 0X1200)//0X022C 681C
#define PLATFORM_BASE_ADDR			(FRISK_RIGHT_1_ADDR + IMG_HEADER_LEN + 0X1200)//0x022C 7A20
#define PLATFORM_LVL1_ADDR			(PLATFORM_BASE_ADDR + IMG_HEADER_LEN + 0x2A50)//0x022 CA474

#define SUCCESS 	 0
#define FAILURE 	-1

extern void xil_printf(const char *format, ...);

typedef unsigned char bool;

#endif
