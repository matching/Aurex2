/*
 * cmlcdpixdefault.cpp - a pixel control class for default (= not anti aliases)
 *
 * $Id: cmlcdpixdefault.cpp,v 1.9 2006/02/19 13:04:40 fumi Exp $
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
#include <assert.h>


#include "cmlcdpix.h"
#include "cmlcdpixdefault.h"

#include "lcdblrvmgr.h"

CMLcdPixDefault::CMLcdPixDefault(int expansion, int ai_value) : CMLcdPix(expansion)
{
	pixelData   = new unsigned char * [realHeight];

	int i;
	for( i = 0; i < realHeight; i++ )
	{
		pixelData[i] = new unsigned char [realWidth];
		memset(pixelData[i], 0, realWidth);
	}


	this->ai_value = ai_value;

	if( ai_value > 0 )
	{
		int j;
		for( i = 0; i < 256; i++ )
		{
			for( j = 0; j < 256; j++ )
				aiTransientTable[ i ][ j ] = ( j + i * ( ai_value - 1 ) ) / ai_value ;
		}
	}
}


CMLcdPixDefault::~CMLcdPixDefault(void)
{
	int i;
	for( i = 0; i < realHeight; i++ )
	{
		delete[] pixelData[i];
	}

	delete[] pixelData;

	pixelData = NULL;
}


/** LCDの点の明暗を設定する
 * ただしメンバ変数に敢えて直接触れる設計にしてあるので、
 * 必要無いかもしれない
 * @param x x 座標
 * @param y y 座標
 * @param color 0:白/1:黒
 */
void CMLcdPixDefault::pset(int x, int y, int color)
{
	if(x < 0 || x >= 320 || y < 0 || y >= 240)
		return;

	assert( color == 0 || color == 1 );


	now[y][x] = color;


	int i, j;
	int realX = convertToReal( x ), realY = convertToReal( y );

	color = ( color ? 255 : 0 );

	for(i = 0; i < expansion; i++)
		for(j = 0; j < expansion; j++)
			pixelData[ realY + i][ realX + j] = color;
}


void CMLcdPixDefault::psetReal(int x, int y, int color)
{
	if(x < 0 || x >= getRealWidth320()
	   || y < 0 || y >= getRealHeight240() )
		return;

	pixelData[y][x] = color;

	CMLcdPix::pset( convertFromReal( x ), convertFromReal( y ), (color >= 96 ? 1 : 0));
}

int CMLcdPixDefault::pgetReal(int x, int y)
{
	if(x < 0 || x >= getRealWidth320()
	   || y < 0 || y >= getRealHeight240() )
		return 0;

	return 	pixelData[y][x];
}

void CMLcdPixDefault::updatePixel(int start_x, int start_y, enum UpdateRegion updateRegion)
{
	int x, y;
	int end_y, end_x;
	int place_x, place_y;

	start_x = convertToReal( start_x );
	start_y = convertToReal( start_y );

	if(updateRegion == PART)
	{
		end_x = getRealWidth102();
		end_y = getRealHeight64();
	}
	else
	{
		end_y = getRealHeight240();
		end_x = getRealWidth320();
	}

#if 1
	for( y = 0; y < end_y; y++ )
	{
		place_y = start_y + y;

		for( x = 0; x < end_x; x++ )
		{
			place_x = start_x + x;
			switch(LcdBlRvMgr::getPixelStatus( convertFromReal( place_x ), convertFromReal( place_y )))
			{
			case ON:
				fgPixelData[y][x] = aiTransientTable[ fgPixelData[y][x] ][ pixelData[place_y][place_x] ];
				break;

			case OFF:
				fgPixelData[y][x] = aiTransientTable[ fgPixelData[y][x] ][ 0 ];
				break;

			case REVERSE:
				fgPixelData[y][x] = aiTransientTable[ fgPixelData[y][x] ][ 255 - pixelData[place_y][place_x] ];
				break;
			}
		}
	}
#else
	for( y = start_y; y < end_y; y++ )
	{
		for( x = start_x; x < end_x; x++ )
		{
			switch(LcdBlRvMgr::getPixelStatus( convertFromReal( x ), convertFromReal( y )))
			{

			case ON:
				fgPixelData[y - start_y][x - start_x] = aiTransientTable[ fgPixelData[y - start_y][x - start_x] ][ pixelData[y][x]];
				break;

			case OFF:
				fgPixelData[y - start_y][x - start_x] = aiTransientTable[ fgPixelData[y - start_y][x - start_x] ][ 0 ];
				break;

			case REVERSE:
				fgPixelData[y - start_y][x - start_x] = aiTransientTable[ fgPixelData[y - start_y][x - start_x] ][ 255 - pixelData[y][x] ];
				break;
			}
		}
	}
#endif

}
