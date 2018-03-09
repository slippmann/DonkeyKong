/*
 * video.h
 *
 *  Created on: Mar 7, 2018
 *      Author: slippman
 */

#ifndef _VIDEO_H_
#define _VIDEO_H_

#include "../global.h"

struct image {
	u16 width;
	u16 height;
	u16 data[];
};

void CopyImage(struct image * dest, struct image * src);
void DrawImage(u16 * frameptr, struct image * sprite, u16 x, u16 y);

#endif /* _VIDEO_H_ */
