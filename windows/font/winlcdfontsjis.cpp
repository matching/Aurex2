/* -*-coding:sjis-unix-*-
 *
 * winlcdfontsjis.cpp -
 *
 * $Id: winlcdfontsjis.cpp,v 1.7 2006/01/19 17:34:35 fumi Exp $
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
#include "cessjis.h"

WINLcdFontSjis::WINLcdFontSjis(HDC hDC_wnd, int width, int height, int prop, int gap) : WINLcdFont(hDC_wnd, width, height, prop, gap, "‚l‚r ƒSƒVƒbƒN", "‚l‚r ‚oƒSƒVƒbƒN", SHIFTJIS_CHARSET)
{
	sjis=new CesSjis();
}
WINLcdFontSjis::~WINLcdFontSjis(void)
{
	delete sjis;
}

unsigned char **WINLcdFontSjis::drawString(char const *str, int *resultwidth)
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
		int b;
		int x=0;
		while(*str)
		{
			b=sjis->checkChar(str);
			TextOut(hDC, x, 0, str, b);
			x+=fwidth+gap;
			if(b == 2)
				x+=fwidth;
			str+=b;
		}
	}

	int i;
	unsigned char **buf;
	buf=new unsigned char * [fheight];
	for(i=0; i < fheight; i++)
		buf[i]=(dibData+i*320);

	return buf;
}
