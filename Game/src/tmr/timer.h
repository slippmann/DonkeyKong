#ifndef _TIMER_H_
#define _TIMER_H_

#include "../global.h"

#include "xparameters.h"
#include "xscutimer.h"
#include "xil_exception.h"
#include "../int/interrupts.h"

#define TIMER_DEVICE_ID	 	XPAR_PS7_SCUTIMER_0_DEVICE_ID
#define TIMER_BASE_ADDR		XPAR_PS7_SCUTIMER_0_BASEADDR
#define TIMER_IRPT_INTR		XPAR_SCUTIMER_INTR
//#define TIMER_LOAD_VALUE	0x17D78400 //2FAF0800 every 2 seconds

int InitTimer(void);
void StartTimer(u16 timerLoadValue, bool autoReload, void (*callback)(void * data));
void StopTimer(void);
void DestroyTimer(void);

#endif /* _TIMER_H_ */
