#include "video.h"

/*****************************************************************************/
/**
*
* This function copies image data from one section of memory on top of an
* existing frame
*
* @param	frame 	- pointer to the existing frame
* @param	sprite 	- pointer to the image to copy
* @param	width 	- width of the sprite image
* @param	height 	- height of the sprite image
* @param	x		- horizontal position on existing frame to start
* 					  drawing sprite [top left corner of screen is (0, 0)]
* @param	y		- vertical position on existing frame to start
* 					  drawing sprite [top left corner of screen is (0, 0)]
*
* @return
*		- XST_SUCCESS if successful,
*		- XST_FAILURE.if not successful
*
* @note		None.
*
******************************************************************************/
void DrawImage(u16 * frameptr, struct image * img, u16 x, u16 y)
{
	u32 frameOffset = 0;
	u32 imgOffset = 0;
	u16 width = img->width;
	u16 height = img->height;
	u16 adjustedWidth = width;
    u16 adjustedHeight = height;

	// handle x overflow
	if(x + width > FRAME_WIDTH)
	{
		adjustedWidth = FRAME_WIDTH - x;
	}

	// handle y overflow
	if(y + height > FRAME_HEIGHT)
	{
		adjustedHeight = FRAME_HEIGHT - y;
	}

	u32 yOffset = y * FRAME_WIDTH;

	for(int yi = 0; yi < adjustedHeight; yi++)
	{
		imgOffset = yi * width;
		imgOffset += IMG_HEADER_LEN;

		frameOffset = (yOffset + (yi * FRAME_WIDTH)) + x;

		for(int xi = 0; xi < adjustedWidth; xi++)
		{
			//check for alpha channel
			if (img->data[imgOffset + xi] & 0xF000)
			{
				frameptr[frameOffset + xi] = img->data[imgOffset + xi];
			}
		}
	}
}

/*****************************************************************************/
/**
*
* This function copies image data from one section of memory to another
*
* @param	dest is a pointer to where the image should be copied
* @param	src is a pointer to the image to copy
*
* @return
*		- XST_SUCCESS if successful,
*		- XST_FAILURE.if not successful
*
* @note		None.
*
******************************************************************************/
void CopyImage(struct image * dest, struct image * src)
{
	u16 width = src->width;
	u16 height = src->height;
	u32 dataLength = (width * height * 2) + IMG_HEADER_LEN;

	dest->width = width;
	dest->height = height;
	memcpy((void *)dest->data, (const void *)src->data, dataLength);
}


