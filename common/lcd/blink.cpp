/*
 * blink.cpp - Blink module
 *
 * $Id: blink.cpp,v 1.7 2005/01/06 14:21:33 fumi Exp $
 *
 * Copyright (C) 2004 Fumihiko MACHIDA <machida@users.sourceforge.jp>
 * All rights reserved.
 *
 * This is free software with ABSOLUTELY NO WARRANTY.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA
 */


#include <stdio.h>
#include <stdlib.h>

#include <list>
using namespace std;

#include "blink.h"

char Blink::blink_count_reset[]={32, 32, 16, 96};	/* 1/64 をかけると秒数だったり */

Blink::Blink(int x1, int y1, int x2, int y2, int color) : LcdBlRv(x1, y1, x2, y2)
{
	on=1;
	type=(color >> 4) & 0x3;
	hz=(color >> 6) & 0x3;

	count=blink_count_reset[hz];


#ifdef DEBUG
	fprintf(stderr, "\
-----------blink ----------\n\
region: (%d, %d)-(%d, %d)\n\
color : %x\n\
---------------------------\n",
			x1, y1, x2, y2, color);
	fflush(stderr);
#endif
}

// 64 Hz 毎に呼ばれるー
int Blink::proceed(void)
{
	count--;
	if(count <= 0)
	{
// トグル動作
		if(hz == 0 || hz == 3)
			hz^=0x3;

		count=blink_count_reset[hz];


		on^=1;
	}

	switch(type)
	{
	case 1: /* invert */
		if(on == 0)
			invert();
		else
			pixel_on();
		break;
	case 2:	/* on / off */
		if(on == 1)
			off();
		else
			pixel_on();
		break;
	}

	return 0;
}

