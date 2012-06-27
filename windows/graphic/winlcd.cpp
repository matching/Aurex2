/*
 * winlcd.cpp - windows lcd module
 *
 * $Id: winlcd.cpp,v 1.24 2006/02/10 17:25:26 fumi Exp $
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

#include "winlcd.h"
#include "winlcdfont.h"
#include "cmlcd.h"
#include "cmlcdfunc.h"


#include <stdio.h>
#include <assert.h>


WINLcd::WINLcd(HWND hWnd, CMLcd *cmlcd, int width, int height) : SDLcd(cmlcd)
{
	assert( cmlcd );

	this->wwidth = width;
	this->wheight = height;
	this->hWnd = hWnd;

	init();

}

WINLcd::WINLcd(HWND hWnd, CMLcd *cmlcd) : SDLcd(cmlcd)
{
	assert( cmlcd );

	cmlcd->updateAllPixel();

	this->wwidth = cmlcd->getRealWidth320();
	this->wheight = cmlcd->getRealHeight240();
	this->hWnd = hWnd;

	init();
}

WINLcd::~WINLcd()
{
	delete[] (char *)bitmapInfo;

	SelectObject(hDC, prev_obj);

	DeleteObject(hBitmap);
	DeleteDC(hDC);
	ReleaseDC(hWnd, hDC_wnd);
}

unsigned char WINLcd::createColor(int bit)
{
#if 0
	int i;
	int c=255;

	for(i=0; i < 8; i++)
	{
		if(bit & (1 << i))
		{
			c-=128;
			if(c <= 0)
				c=0;
		}
		else
			c=255-(255-c)/4;
	}

	return c;
#endif
	return 255 - bit;
}


void WINLcd::init(void)
{
	int i;

//	adjustWindowSize();
//

	RGBQUAD *q;

	bitmapInfo=(BITMAPINFO *)(new char[sizeof(BITMAPINFO)+256*4]);
	memset(bitmapInfo, 0, sizeof(BITMAPINFOHEADER)+256*4);

	bitmapInfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bitmapInfo->bmiHeader.biWidth=wwidth;
	bitmapInfo->bmiHeader.biHeight=-wheight;
	bitmapInfo->bmiHeader.biPlanes=1;
	bitmapInfo->bmiHeader.biBitCount=8;

	q=bitmapInfo->bmiColors;

	for(i=0; i < 256; i++, q++)
	{
		q->rgbBlue		=
			q->rgbGreen	=
			q->rgbRed	= createColor(i);
	}

	{
		hDC_wnd=GetDC(hWnd);

		hDC=CreateCompatibleDC(hDC_wnd);

		hBitmap=CreateDIBSection(hDC_wnd, bitmapInfo, DIB_RGB_COLORS, (void **)&dibData, NULL, 0);
		prev_obj=SelectObject(hDC, hBitmap);

// 4 byte 境界にする
		byte_per_line = wwidth;
		if(byte_per_line % 4)
			byte_per_line = wwidth + (4 - (wwidth % 4));

		memset(dibData, 0, byte_per_line * wheight);
	}
}

void WINLcd::updateWindow(void)
{
	int y;
	unsigned char const *p;

	int realHeight;
	int realWidth;

	realHeight = cmlcd->getRealHeight240();
	realWidth  = cmlcd->getRealWidth320();

	for(y=0; y < realHeight; y++)
	{
		p = cmlcd->getCMLcdFunc()->getPixelArray(0, y);
		memcpy(dibData + y * byte_per_line, p, realWidth);
	}

	GdiFlush();
	BitBlt(hDC_wnd, 0, 0, realWidth, realHeight, hDC, 0, 0, SRCCOPY);

//	memcpy(dibData, cmlcd->hist, 320*240);
#if 0
	for(x=0; x < 320; x++)
		for(y=0; y < 240; y++)
			*((unsigned char *)(dibData+y*320+x))=cmlcd->getHist(x, y);
#endif
// ???
//	GdiFlush();
//	BitBlt(hDC_wnd, 0, 0, 320, 240,	hDC, 0, 0, SRCCOPY);
#if 0
#define CAPTUREBLT 0x40000000
	BitBlt(hDC_wnd, 0, 0, 320, 240,	hDC, 0, 0, SRCCOPY | CAPTUREBLT);
#endif
}



SDLcdFont *WINLcd::createSDLcdFont(int width, int height, int prop, int gap, bool latin)
{
	if(latin)
		return new WINLcdFont(hDC_wnd, width, height, prop, gap);
	else
		return new WINLcdFontSjis(hDC_wnd, width, height, prop, gap);
}

SDLcdFont *WINLcd::createSDLcdFontAa(int width, int height, int prop, int gap, bool latin)
{
	if(latin)
		return new WINLcdFontAa(hDC_wnd, width, height, prop, gap);
	else
		return new WINLcdFontAaSjis(hDC_wnd, width, height, prop, gap);
}

void WINLcd::adjustWindowSize(void)
{
	adjustWindowSize(0, 0);
}

void WINLcd::adjustWindowSize(int w, int h)
{
	RECT r={0, 0, wwidth + w, wheight + h};
	DWORD style;


	style=GetWindowLong(hWnd, GWL_STYLE);

	AdjustWindowRect(&r, style, FALSE);

#ifdef DEBUG
	fprintf(stderr, "!! adust (%d, %d)\n", r.right-r.left, r.bottom-r.top);
	fflush(stderr);
#endif

	SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, r.right-r.left, r.bottom-r.top, SWP_NOMOVE | SWP_NOZORDER);
}



bool WINLcd::hasFontAa(void)
{
	return true;
}
