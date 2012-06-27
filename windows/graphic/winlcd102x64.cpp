/*
 * winlcd102x64.cpp -
 *
 * $Id: winlcd102x64.cpp,v 1.12 2006/02/10 17:25:26 fumi Exp $
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

#include <assert.h>

#include "winlcd.h"
#include "cmlcd.h"
#include "cmlcdfunc.h"
#include "rupaddress.h"

//WINLcd102x64::WINLcd102x64(RupAddress *systemAddress, HWND hWnd, CMLcd *cmlcd) : WINLcd(hWnd, cmlcd, 102, 64)
//WINLcd102x64::WINLcd102x64(RupAddress *systemAddress, HWND hWnd, CMLcd *cmlcd) : WINLcd(hWnd, cmlcd, 204, 128)
WINLcd102x64::WINLcd102x64(RupAddress *systemAddress, HWND hWnd, CMLcd *cmlcd) : WINLcd(hWnd, cmlcd, cmlcd->getRealWidth102(), cmlcd->getRealHeight64())
{
	this->systemAddress = systemAddress;

	assert( systemAddress );

	cmlcd->updatePartPixel();
}

WINLcd102x64::~WINLcd102x64(void)
{
// do nothing
}

void WINLcd102x64::updateWindow(void)
{
	int y;
	int pos_x, pos_y;
	unsigned char const *p;

	systemAddress->get_lcdPlace(&pos_x, &pos_y);

	pos_x *= cmlcd->getExpansion();
	pos_y *= cmlcd->getExpansion();

	int end_y;
	int realWidth;

	end_y = cmlcd->getRealHeight64();
	realWidth = cmlcd->getRealWidth102();

	for(y=0; y < end_y; y++)
	{
//		p = cmlcd->getCMLcdFunc()->getPixelArray(pos_x, y + pos_y);
		p = cmlcd->getCMLcdFunc()->getPixelArray(0, y);
		memcpy(dibData + y * byte_per_line, p, realWidth);
	}

	GdiFlush();
	BitBlt(hDC_wnd, 0, 0, cmlcd->getRealWidth102(), cmlcd->getRealHeight64(), hDC, 0, 0, SRCCOPY);
}
