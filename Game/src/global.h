#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "xparameters.h"
#include "xil_exception.h"
#include "xdebug.h"
#include "string.h"

#define DEBUG

#define DMA_DEV_ID			    XPAR_AXIDMA_0_DEVICE_ID
#define DDR_BASE_ADDR			XPAR_PS7_DDR_0_S_AXI_BASEADDR
#define MEM_BASE_ADDR		  	(DDR_BASE_ADDR + 0x1000000)

#define FRAME_WIDTH 			(640)
#define FRAME_HEIGHT			(480)
#define FRAME_LEN				(FRAME_WIDTH * FRAME_HEIGHT * 2) /* 640x480 = 307200 pixels * 2 bytes/pixel= 614400 bytes */

#define VIDEO_MEM_SIZE		 	(FRAME_LEN * 2) /* two frames */

#define GAME_BASE_ADDR			(MEM_BASE_ADDR + VIDEO_MEM_SIZE)

#define IMG_HEADER_LEN			(4)
#define CONTENT_BASE_ADDR		(DDR_BASE_ADDR + 0x2000000)

#define SUCCESS 	 0
#define FAILURE 	-1

extern void xil_printf(const char *format, ...);

typedef unsigned char bool;

#endif
