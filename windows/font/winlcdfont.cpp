/*
 * winlcdfont.cpp -
 *
 * $Id: winlcdfont.cpp,v 1.8 2006/01/21 10:02:03 fumi Exp $
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

#include <windows.h>
#include "winlcdfont.h"
#include "sdlcdfont.h"
#include "cmlcd.h"

#include <assert.h>

WINLcdFont::WINLcdFont(HDC hDC_wnd, int width, int height, int prop, int gap, char const *fixedfont, char const *propfont, DWORD charset)
{
	this->gap=gap;
	this->fwidth=width;
	this->fheight=height;

	bitmapInfo=(BITMAPINFO *)(new char[sizeof(BITMAPINFO)+256*4]);
	memset(bitmapInfo, 0, sizeof(BITMAPINFOHEADER)+256*4);

	bitmapInfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bitmapInfo->bmiHeader.biWidth=320; // ºÇÂçÉý
	bitmapInfo->bmiHeader.biHeight=-32;
	bitmapInfo->bmiHeader.biPlanes=1;
	bitmapInfo->bmiHeader.biBitCount=8;

	RGBQUAD *q;
	q=bitmapInfo->bmiColors;
	int i;
	for(i=0; i < 256; i++, q++)
	{
			q->rgbBlue=i;
			q->rgbGreen=i;
			q->rgbRed=i;

	}

	hBitmap=CreateDIBSection(hDC_wnd, bitmapInfo,
							 DIB_RGB_COLORS, (void **)&dibData, NULL, 0);
	memset(dibData, 0xff, 320*32);

	hDC=CreateCompatibleDC(hDC_wnd);
	if ( prop )
		hFont=CreateFont(height, width * 3 / 4, 0, 0, FW_DONTCARE,
						 FALSE, FALSE, FALSE, charset,
						 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
						 VARIABLE_PITCH, propfont);
	else
		hFont=CreateFont(height, width, 0, 0, FW_DONTCARE,
						 FALSE, FALSE, FALSE, charset,
						 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
						 FIXED_PITCH, fixedfont);
	if(hFont)
	{
		prev_obj=SelectObject(hDC, hBitmap);
		SelectObject(hDC, hFont);
	}

	assert( hFont );
}

WINLcdFont::~WINLcdFont(void)
{
	delete[] (char *)bitmapInfo;

	DeleteObject(hBitmap);
	DeleteObject(hFont);
	DeleteDC(hDC);
}

unsigned char **WINLcdFont::drawString(char const *str, int *resultwidth)
{
	int len;
	len=strlen(str);

	if(gap == 0)
	{
		TextOut(hDC, 0, 0, str, len);
		if(resultwidth)
			*resultwidth=len*fwidth;
	}
	else
	{
		if(resultwidth)
			*resultwidth=len*(fwidth+gap);

		int x;
		x=0;
		while(len--)
		{
			TextOut(hDC, x, 0, str, 1);
			x+=fwidth+gap;
			str++;
		}

	}


	int i;
	unsigned char **buf;
	buf=new unsigned char * [fheight];
	for(i=0; i < fheight; i++)
		buf[i]=(dibData+i*320);


//  ????
//	GdiFlush();

	return buf;
}

void WINLcdFont::freeBuffer(unsigned char **buf)
{
	delete[] buf;
}
