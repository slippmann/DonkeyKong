#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "xparameters.h"
#include "xil_exception.h"
#include "xdebug.h"
#include "string.h"


// Device hardware build related constants.
#define DMA_DEV_ID		XPAR_AXIDMA_1_DEVICE_ID
#define DDR_BASE_ADDR	XPAR_PS7_DDR_0_S_AXI_BASEADDR
#define MEM_BASE_ADDR	(DDR_BASE_ADDR + 0x1000000)
#define TX_INTR_ID		XPAR_FABRIC_AXIDMA_1_VEC_ID
#define TX_BUFFER_BASE	(MEM_BASE_ADDR + 0x00100000)

#define INTC_DEVICE_ID  XPAR_SCUGIC_SINGLE_DEVICE_ID
#define INTC			XScuGic
#define INTC_HANDLER	XScuGic_InterruptHandler

// Audio and Audio Descriptor related constants
#define MAX_PKT_LEN				0x2EE00 //192000 bytes i.e. one second
#define GAME_SONG_TRANSFERS		32 //32 transfers for the full game song
#define MENU_SONG_TRANSFERS 	10 //10 transfers for the full menu song
#define GAME_SONG_LENGTH 		0x5F350C //6239500 bytes, length of game song
#define MENU_SONG_LENGTH		0x1D9780 //1939328 bytes, length of menu song

#define SUCCESS 	 0
#define FAILURE 	-1

// Timeout loop counter for reset
#define RESET_TIMEOUT_COUNTER	10000

#ifndef DEBUG
extern void xil_printf(const char *format, ...);
#endif


typedef unsigned char bool;

#endif