/***************************** Include Files *********************************/

#include "xparameters.h"
#include "global.h"
#include "aud/audio.h"

int main(void)
{
	int Status;
	int Index;
	u32 *song = (u32 *)SONG_ADDR;

	xil_printf("\r\n--- Entering main() --- \r\n");

	Status = InitAudio();

	if(Status != SUCCESS)
	{
		return FAILURE;
	}

	//volume decrease (I should really find a different way to do this)
//	for(Index = 0; Index < (GAME_SONG_LENGTH/4); Index ++)
//	{
//		song[Index] = (int)song[Index] >> 0xC;
//	}

	// Play song on loop
	while (true)
	{
		PlaySong(song, GAME_SONG_LENGTH);
		sleep(1);
	}

	DisableInterrupt(AUDIO_INTR_ID);

	xil_printf("--- Exiting main() --- \r\n");

	return SUCCESS;
}
