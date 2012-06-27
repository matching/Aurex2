/*
 * winlcdflip.cpp -
 *
 * $Id: winlcdflip.cpp,v 1.9 2005/01/06 14:21:33 fumi Exp $
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
#include "cmlcd.h"
#include "cmlcdfunc.h"

WINLcdFlip::WINLcdFlip(HWND hWnd, CMLcd *cmlcd) : WINLcd(hWnd, cmlcd, cmlcd->getRealHeight240(), cmlcd->getRealWidth320())
{
}


void WINLcdFlip::updateWindow(void)
{
// フリップしているので、普通の転送は不可

	int x, y;
	unsigned char const *p;

	int realHeight;
	int realWidth;

	realHeight = cmlcd->getRealHeight240();
	realWidth  = cmlcd->getRealWidth320();

	for(y=0; y < realHeight; y++)
	{
		p = cmlcd->getCMLcdFunc()->getPixelArray(0, y);

		for(x=0; x < realWidth; x++, p++)
		{
	//			*(dibData + (realWidth - 1 - x) * byte_per_line + (realHeight - 1 - y)) = *p;
			*(dibData + x * byte_per_line + (realHeight - 1 - y)) = *p;
		}
	}

	GdiFlush();
	BitBlt(hDC_wnd, 0, 0, realHeight, realWidth, hDC, 0, 0, SRCCOPY);

}
