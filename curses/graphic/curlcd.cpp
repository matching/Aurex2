/*
 * curlcd.cpp - curses graphic
 *
 * $Id: curlcd.cpp,v 1.15 2006/02/20 13:58:05 fumi Exp $
 *
 * Copyright (C) 2004, 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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


#include "curlcd.h"
#include "cmlcd.h"
#include "cmlcdfunc.h"
#include <curses.h>
#include <stdio.h>

#include "bdfmgr.h"
#include "bdflcdfontjis.h"
#include "bdffontsets.h"
#include "bdffontsetsjis.h"

CURLcd::CURLcd(CMLcd *cmlcd) : SDLcd(cmlcd)
{
	initscr();
	noecho();
	nodelay(stdscr, TRUE);
	cbreak();
	keypad(stdscr, TRUE);

	bdfmgr=new BdfMgr();
	bdfmgr->add_jisx0201_font("iso-font10.bdf", 10);
	bdfmgr->add_jisx0201_font("iso-font12.bdf", 12);
	bdfmgr->add_jisx0208_font("jis-font10.bdf", 10);
	bdfmgr->add_jisx0208_font("jis-font12.bdf", 12);

	for(int i = 0 ; i < 256; i++)
	{
		if( i >= 0xc0)
			pixelChars[i]='@';
		else if(i >= 0xa0)
			pixelChars[i]='*';
		else if(i >= 0x80)
			pixelChars[i]='+';
		else if(i >= 0x60)
			pixelChars[i]='-';
		else if(i >= 0x40)
			pixelChars[i]=',';
		else if(i >= 0x20)
			pixelChars[i]='.';
		else
			pixelChars[i]=' ';
	}
}

CURLcd::~CURLcd()
{
	endwin();
}

int CURLcd::getBottomLine(void)
{
	int height;

	height = LINES;
	if(height >= 65)
		height = 65;
	else
		height = LINES;

	return height - 1; // to ºÂÉ¸
}

void CURLcd::updateWindow(void)
{
	int x, y;
	unsigned char str[400]="";

	int height;
	int width;
	unsigned char const *p;

#if 1
	width = COLS;
	if(width > 102)
		width = 102;

	height = LINES;
	if(height >= 65)
		height = 64;
	else
		height = LINES - 1;
#else
	width=COLS;
	if(width > 320)
		width=320;

	height=LINES;
	if(height > 240)
		height=240;
#endif

	for(y=0; y < height; y++)
	{
		p = cmlcd->getCMLcdFunc()->getPixelArray(0, y);

		for(x=0; x < width; x++)
		{
			str[x] = pixelChars[ p[x] ];
		}
		mvaddstr(y, 0, (char *)str);
	}

	refresh();
}

SDLcdFont *CURLcd::createSDLcdFont(int width, int height, int prop, int gap, bool latin)
{
#if 1
	BdfFontSets *bfss;
	if(latin)
	{
/*
		bfss=bdfmgr->getBdfFontSetsIso(height);
		return new BdfLcdFont(width, height, gap, bfss);
*/
	}
	else
	{
		bfss=bdfmgr->getJisBdfFont(height);
		if(bfss == NULL)
			return NULL;

		return new BdfLcdFontJis(width, height, gap, bfss);
	}
#endif
	return NULL;
}
