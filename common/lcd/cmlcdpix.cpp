/*
 * cmlcdpix.cpp - a pixel control base class
 *
 * $Id: cmlcdpix.cpp,v 1.4 2006/02/16 18:19:40 fumi Exp $
 *
 * Copyright (C) 2004, 2005 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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


#include "cmlcdpix.h"


#include <vector>
#include <iterator>
using namespace std;

CMLcdPix::CMLcdPix(int expansion)
{
	this->expansion = expansion;

	memset(now, 0, 320 * 240);

	realWidth = 320 * expansion;
	realHeight = 240 * expansion;


	fgPixelData   = new unsigned char * [realHeight];

	int i;
	for( i = 0; i < realHeight; i++ )
	{
		fgPixelData[i] = new unsigned char [realWidth];
		memset(fgPixelData[i], 0, realWidth);
	}

	vectorFromReal = new int [realWidth];
	for( i = 0; i < realWidth; i++)
	{
		vectorFromReal[i] = i / expansion;
	}

	vectorToReal = new int [320]; // 320 is width
	for( i = 0; i < 320; i++)
	{
		vectorToReal[i] = i * expansion;
	}

}

CMLcdPix::~CMLcdPix(void)
{
	int i;
	for( i = 0; i < realHeight; i++ )
	{
		delete[] fgPixelData[i];
	}
	delete[] fgPixelData;

	fgPixelData = NULL;
}

/** LCDの点の明暗を得る
 * @param x x 座標
 * @param y y 座標
 * @retval 0 白
 * @retval 1 黒
 */
int CMLcdPix::pget(int x, int y)
{
	if(x < 0 || x >= 320 || y < 0 || y >= 240)
		return 0;

	return now[y][x];
}

void CMLcdPix::pset(int x, int y, int c)
{
	if(x < 0 || x >= 320 || y < 0 || y >= 240)
		return;

	now[y][x] = c;
}

int CMLcdPix::getRealWidth102(void) const
{
	return 102 * expansion;
}
int CMLcdPix::getRealWidth320(void) const
{
	return realWidth;
}

int CMLcdPix::getRealHeight64(void) const
{
	return 64 * expansion;
}
int CMLcdPix::getRealHeight240(void) const
{
	return realHeight;
}

#if 0
/**
 * 実座標から 320x240 に写像する
 */
int CMLcdPix::convertFromReal(int xy)
{
	return realVector[ xy ];
}
#endif

/**
 * 境界チェックを行なうべきかもしれないが。
 */
unsigned char const *CMLcdPix::getPixelArray(int x, int y)
{
	return fgPixelData[y] + x;
}

/*
 * Todo 浮動小数点はやめる？
 * アンチエイリアスをするのはいいのか？
 */
void CMLcdPix::setPreviousPix(CMLcdPix *pix)
{
	int x, y;
	int end_x, end_y;

	end_x = this->getRealWidth320();
	end_y = this->getRealHeight240();

	memcpy( now, pix->now, 320 * 240 ); // static にすればいいのにね。

	if( this->expansion == pix->expansion )
	{
		for( y = 0; y < end_y; y++ )
			for( x = 0; x < end_x; x++ )
				this->psetReal(x, y, pix->pgetReal(x, y) );
	}
	else if( this->expansion > pix->expansion )
	{
// 拡大
		double r;
		r = (double)pix->expansion / this->expansion ;

		int ix, iy;
		double sx, sy;
		double dx, dy;
		for( y = 0, sy = 0.0; y < end_y; y++, sy += r)
		{
			double dy;

			dy = sy - (int)sy;
			iy = (int)sy;

			for( x = 0, sx = 0.0; x < end_x; x++, sx += r)
			{
				dx = sx - (int)sx;
				ix = (int)sx;

				double v = 0.0;

				v += pix->pgetReal(ix + 0, iy + 0) * (1 - dx) * (1 - dy);
				v += pix->pgetReal(ix + 1, iy + 0) *      dx  * (1 - dy);
				v += pix->pgetReal(ix + 0, iy + 1) * (1 - dx) *      dy ;
				v += pix->pgetReal(ix + 1, iy + 1) *      dx  *      dy ;
#ifdef DEBUG_
				fprintf(stderr, "CMLcdPix::setPreviousPix (%d, %d) = %e, dx:%e, dy:%e, ix:%d, iy:%d, %d\n", x, y, v, dx, dy, ix, iy, pix->pgetReal(ix, iy));
				fflush(stderr);
#endif
				psetReal(x, y, (unsigned char)v);
			}
		}

	}
	else
	{
// 縮小
		double r;
		r = (double)pix->expansion / this->expansion; // r は 1 より大きい

		double sx, sy;
		int ix, iy;
		int end_ix, end_iy;

		for( y = 0, sy = 0.0; y < end_y; y++, sy += r)
		{
			end_iy = sy + r + 1;

			for( x = 0, sx = 0.0; x < end_x; x++, sx += r)
			{
				double v;

				end_ix = sx + r + 1;
				v = 0.0;

#ifdef DEBUG_
				fprintf(stderr, "CMLcdPix::setPreviousPix syukusyo sx:%e, end_ix:%d; sy:%e, end_iy:%d\n", sx, end_ix, sy, end_iy);
				fflush(stderr);
#endif
				double w, h;
				double tx, ty;
				for( iy = (int)sy; iy < end_iy; iy++ )
				{
					ty = iy / r;
					h = 1 / r;
					if( ty < y )
						h -= y - ty;
					else if( ty + 1 / r > y + 1 )
						h -= (ty + 1 / r) - ( y + 1 );

					for( ix = (int)sx; ix < end_ix; ix++ )
					{
						tx = ix / r;

						w = 1 / r;
						if( tx < x )
							w -= x - tx;
						else if( tx + 1 / r > x + 1 )
							w -= (tx + 1 / r) - ( x + 1 );

						v += pix->pgetReal(ix, iy) * w * h;
					}
				}
#ifdef DEBUG_
				fprintf(stderr, "CMLcdPix::setPreviousPix syukusyo (%d, %d) = %e\n", x, y, v);
				fflush(stderr);
#endif
				psetReal(x, y, (unsigned char)v);
			}
		}
	}
}
