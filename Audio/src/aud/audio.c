/*
 * audio.c
 *
 *  Created on: Mar 15, 2018
 *      Author: slippman
 */
#include "audio.h"

// Instance of the XAxiDma
static XAxiDma AxiDma;

// Flags interrupt handlers use to notify the application context the events.
volatile int TxDone;
volatile int Error;

static int DMAConfig(void);
static void TxIntrHandler(void *Callback);

int InitAudio(void)
{
	int Status;
	Error = 0;

	Status = DMAConfig();
	if (Status != SUCCESS)
	{
		xil_printf("DMA configuration failed %d\r\n", Status);
		return FAILURE;
	}

	// Initialize flags before start transfer test
	TxDone = 0;
	Error = 0;

	return SUCCESS;
}

/*****************************************************************************/
/**
*
* This function configures the DMA engine
*
* @param	None.
*
* @return
*		- XST_SUCCESS if successful,
*		- XST_FAILURE.if not successful
*
* @note		None.
*
******************************************************************************/
int DMAConfig(void)
{
	XAxiDma_Config *config;

	config = XAxiDma_LookupConfig(DMA_DEV_ID);

	if (!config)
	{
		xil_printf("No config found for %d\r\n", DMA_DEV_ID);

		return XST_FAILURE;
	}

	/* Initialize DMA engine */
	int Status = XAxiDma_CfgInitialize(&AxiDma, config);

	if (Status != SUCCESS)
	{
		xil_printf("Initialization failed %d\r\n", Status);
		return FAILURE;
	}

	/* Disable all interrupts before setup */
	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

	/* Set up Interrupt system  */
	Status = EnableInterrupts(&AxiDma, AUDIO_INTR_ID, TxIntrHandler, 0xA0);
	if (Status != SUCCESS)
	{
		xil_printf("Failed intr setup\r\n");
		return FAILURE;
	}

	/* Enable all interrupts */
	XAxiDma_IntrEnable(&AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

	return SUCCESS;
}

void PlaySong(u32 * song, u32 length)
{
	int Status;
	u32 * index = song;
	int numTransfers = length / MAX_PKT_LEN;
	int lastPacketLen = length % MAX_PKT_LEN;

	// Flush the SrcBuffer before the DMA transfer, in case the Data Cache is enabled
	Xil_DCacheFlushRange((UINTPTR) song, length);

	for (int i = 0; i < numTransfers; i++)
	{
		int packetLen = i == (numTransfers - 1) ? lastPacketLen : MAX_PKT_LEN;

		TxDone = 0;

		Status = XAxiDma_SimpleTransfer(&AxiDma, (UINTPTR) index, packetLen, XAXIDMA_DMA_TO_DEVICE);

		if (Status != SUCCESS)
		{
			xil_printf("Failed: DMA transfer error");
			return;
		}

		// Wait TX done and RX done
		while (!TxDone && !Error)
		{
			// do nothing
		}

		if (Error)
		{
			xil_printf("Failed: DMA transfer error");
			return;
		}

		index += (MAX_PKT_LEN / 4);
	}
}

/*****************************************************************************/
/*
 *
 * This is the DMA TX Interrupt handler function.
 *
 * It gets the interrupt status from the hardware, acknowledges it, and if any
 * error happens, it resets the hardware. Otherwise, if a completion interrupt
 * is present, then sets the TxDone.flag
 *
 * @param	Callback is a pointer to TX channel of the DMA engine.
 *
 * @return	None.
 *
 * @note	None.
 *
 ******************************************************************************/
static void TxIntrHandler(void *Callback)
{

	u32 IrqStatus;
	int TimeOut;
	XAxiDma *AxiDmaInst = (XAxiDma *) Callback;

	/* Read pending interrupts */
	IrqStatus = XAxiDma_IntrGetIrq(AxiDmaInst, XAXIDMA_DMA_TO_DEVICE);

	/* Acknowledge pending interrupts */
	XAxiDma_IntrAckIrq(AxiDmaInst, IrqStatus, XAXIDMA_DMA_TO_DEVICE);

	/* If no interrupt is asserted, we do not do anything */
	if (!(IrqStatus & XAXIDMA_IRQ_ALL_MASK))
	{

		return;
	}

	/*
	 * If error interrupt is asserted, raise error flag, reset the
	 * hardware to recover from the error, and return with no further
	 * processing.
	 */
	if ((IrqStatus & XAXIDMA_IRQ_ERROR_MASK))
	{

		Error = 1;

		/*
		 * Reset should never fail for transmit channel
		 */
		XAxiDma_Reset(AxiDmaInst);

		TimeOut = RESET_TIMEOUT_COUNTER;

		while (TimeOut)
		{
			if (XAxiDma_ResetIsDone(AxiDmaInst))
			{
				break;
			}

			TimeOut -= 1;
		}

		return;
	}

	/*
	 * If Completion interrupt is asserted, then set the TxDone flag
	 */
	if ((IrqStatus & XAXIDMA_IRQ_IOC_MASK))
	{

		TxDone = 1;
	}
}

