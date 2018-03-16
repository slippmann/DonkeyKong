#include "timer.h"

//global variables
static XScuTimer Timer;
static bool isInitialized = false;

static XScuTimer_Config* ConfigPtr;
volatile int TimerExpired;

static void (*InterruptHandler)(void * data);

//function prototypes
static int ConfigTimer(void);
static void TimerIntrHandler(void *CallBackRef);

#ifndef DEBUG
extern void xil_printf(const char *format, ...);
#endif

int InitTimer(void)
{
	int Status;

	Status = ConfigTimer();

	if (Status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	isInitialized = true;

	return SUCCESS;
}

int ConfigTimer(void)
{
	int Status;

	ConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);

	if (!ConfigPtr)
	{
		return XST_FAILURE;
	}

	Status = XScuTimer_CfgInitialize(&Timer, ConfigPtr, ConfigPtr->BaseAddr);
	if (Status != SUCCESS)
	{
		//xil_printf("Initialization failed %d\r\n", Status);
		return FAILURE;
	}

	Status = EnableInterrupts(&Timer, TIMER_IRPT_INTR, TimerIntrHandler, 0xA0);
	if (Status != SUCCESS)
	{
		//xil_printf("Failed intr setup\r\n");
		return FAILURE;
	}

	XScuTimer_EnableInterrupt(&Timer);

	return SUCCESS;
}

void StartTimer(u16 timerLoadMs, bool autoReload, void (*callback)(void * data))
{
	u32 timerLoadValue = timerLoadMs * 400000;

	if(!isInitialized)
		InitTimer();

	XScuTimer_Stop(&Timer);

	InterruptHandler = callback;

	if (autoReload == true)
	{
		XScuTimer_EnableAutoReload(&Timer);
	}

	else
	{
		XScuTimer_DisableAutoReload(&Timer);
	}

	XScuTimer_LoadTimer(&Timer, timerLoadValue);
	XScuTimer_Start(&Timer);

	return;
}

void StopTimer(void)
{
	XScuTimer_Stop(&Timer);
	XScuTimer_DisableAutoReload(&Timer);

	InterruptHandler = NULL;
}

static void TimerIntrHandler(void *CallBackRef)
{
	XScuTimer *TimerInstancePtr = (XScuTimer *) CallBackRef;

	if (XScuTimer_IsExpired(TimerInstancePtr))
	{
		XScuTimer_ClearInterruptStatus(TimerInstancePtr);
		xil_printf("Timer Event!\n");
		TimerExpired++;
	}

	InterruptHandler(CallBackRef);
}

void DestroyTimer()
{
	DisableInterrupt(TIMER_IRPT_INTR);
}
