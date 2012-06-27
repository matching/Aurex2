/*
 * winlcdfact.cpp - windows ruputer emulation
 *
 * $Id: winlcdfactory.cpp,v 1.8 2005/01/14 04:09:40 fumi Exp $
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

#include "winlcd.h"
#include "winlcdfactory.h"
#include "cmlcd.h"

#include <windows.h>


WINLcdFactory::WINLcdFactory(WidgetOption *option) : SDLcdFactory(option)
{
}

WINLcdFactory::~WINLcdFactory(void)
{
// do nothing
}

WINLcd *WINLcdFactory::getWINLcd(RupAddress *systemAddress, HWND hWnd, CMLcd *cmlcd)
{
	assert( systemAddress );
	assert( hWnd );
	assert( cmlcd );

	changed=false;

	switch(getType())
	{
	case 0:
		return new WINLcd102x64(systemAddress, hWnd, cmlcd);
	case 1:
		return new WINLcd(hWnd, cmlcd);
	case 2:
	case 3:
		return new WINLcdFlip(hWnd, cmlcd);
	}

	return NULL;
}

int WINLcdFactory::getType(void)
{
	return ((int)getFlipFlag() << 1) | getFullWindowFlag();
}

int WINLcdFactory::getWidth(void)
{
	switch(getType())
	{
	case 0: // mini, normal
		return 102;
	case 1: // full, normal
		return 320;
	case 2: // mini, flip
		return 64;
	case 3: // full, flip
		return 240;
	}
	return 320;
}

int WINLcdFactory::getHeight(void)
{
	switch(getType())
	{
	case 0: // mini, normal
		return 64;
	case 1: // full, normal
		return 240;
	case 2: // mini, flip
		return 102;
	case 3: // full, flip
		return 320;
	}
	return 240;
}

