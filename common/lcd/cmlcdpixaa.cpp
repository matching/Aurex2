/*
 * cmlcdpixaa.cpp - a pixel control class for anti aliases
 *
 * $Id: cmlcdpixaa.cpp,v 1.5 2006/02/19 13:04:40 fumi Exp $
 *
 * Copyright (C) 2004, 2005, 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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


/**
 * デフォルトでは使用されない。
 * 使うときは DOT_ANTI_ALIASES を定義すること
 *
 */

#include <stdio.h>

#include "cmlcdpix.h"
#include "cmlcdpixaa.h"
#include "aapixel.h"

#include "lcdblrvmgr.h"

CMLcdPixAA::CMLcdPixAA(int expansion, int ai_value) : CMLcdPixDefault(expansion, ai_value)
{
	aapixel = new AAPixel(expansion);
}


CMLcdPixAA::~CMLcdPixAA(void)
{
	delete aapixel;
}


/** LCDの点の明暗を設定する
 * ただしメンバ変数に敢えて直接触れる設計にしてあるので、
 * 必要無いかもしれない
 * @param x x 座標
 * @param y y 座標
 * @param color 0:白/1:黒
 */
void CMLcdPixAA::pset(int x, int y, int color)
{
	if(x < 0 || x >= 320 || y < 0 || y >= 240)
		return;

/*
	int ret;
	ret = pget(x, y);
	if( color == ret )
		return;
*/
#if 1
	if( color == now[y][x] )
		return;
#endif

	now[y][x] = color;

	x = convertToReal( x );
	y = convertToReal( y );

	int i, i_end, i_offset;
	int j, j_end, j_offset;

	if( y < aapixel->getOffsetTop())
	{
		i_offset = aapixel->getOffsetTop() - y;
		i_end = aapixel->getAllHeight() - i_offset;
	}
	else if( y > realHeight - aapixel->getAllHeight() )
	{
		i_offset = 0;
		i_end = realHeight - y;
	}
	else
	{
		i_offset = 0;
		i_end = aapixel->getAllHeight();
	}

	if( x < aapixel->getOffsetLeft() )
	{
		j_offset = aapixel->getOffsetLeft() - x;
		j_end = aapixel->getAllWidth() - j_offset;
	}
	else if( x > realWidth - aapixel->getAllWidth() )
	{
		j_offset = 0;
		j_end = realWidth - x;
	}
	else
	{
		j_offset = 0;
		j_end = aapixel->getAllWidth();
	}


#if 0 
	fprintf(stderr, "pixAA (x, y)  %d (% 4d, % 4d)-(% 4d, % 4d) %p\n", expansion, x, y, x + j_end, y + i_end, pixelData[0] );
	fflush(stderr);
#endif
	for(i = 0; i < i_end; i++)
	{
		unsigned char const *p;
		int c;

		p = aapixel->getPixelLine(i + i_offset);
		p += j_offset;
		for(j = 0; j < j_end; j++, p++)
			if(color == 0)
			{
				c = (int)(pixelData[ y + i ][ x + j ]) - *p;
				if( c <= 0 )
					c = 0;
				pixelData[ y + i ][ x + j ] = c;
			}
			else
			{
				c = (int)(pixelData[ y + i ][ x + j ]) + *p;
				if( c >= 256 )
					c = 255;
				pixelData[ y + i ][ x + j ] = c;
			}
	}
}
