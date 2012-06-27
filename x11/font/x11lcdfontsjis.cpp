/*
 * x11lcdfontsjis.cpp - X11 font for Japanese font
 *
 * $Id: x11lcdfontsjis.cpp,v 1.5 2006/02/11 19:17:23 fumi Exp $
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


#include "x11lcd.h"
#include "x11lcdfont.h"
#include "x11fontset.h"
#include "cmlcd.h"
#include "cessjis.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


X11LcdFontSjis::X11LcdFontSjis(Display *_display, Window _window, int _width, int _height, int ascent, int gap, XFontSet fontset, int maxWidth, int maxHeight) : X11LcdFont(_display, _window, _width, _height, ascent, gap, fontset, maxWidth, maxHeight)
{
	sjis=new CesSjis();
}

X11LcdFontSjis::~X11LcdFontSjis()
{
	delete sjis;
}

unsigned char **X11LcdFontSjis::drawString(char const *str, int *resultwidth)
{
	char str2[1024], *s;
	XImage *p;

	XSetForeground(display, gc, white);
	XFillRectangle(display, pixmap, gc, 0, 0, maxWidth, maxHeight);
	XSetForeground(display, gc, black);

	sjis->convToEuc(str, str2);

	int len=strlen(str2);
	int widthmax;

#ifdef DEBUG
	fprintf(stderr, "!! (%s)\n", str2);
	fflush(stderr);
#endif

	if(gap == 0)
	{
		XmbDrawString(display, pixmap, fontset, gc, 0, ascent, (char *)str2, len);
		widthmax=len*(width+gap);
	}
	else
	{
		int x=0;
		s=str2;
		while(*s)
		{
			if(*s & 0x80)
			{
				if(*(s+1) == 0)
					break;
				XmbDrawString(display, pixmap, fontset, gc, x, ascent, (char *)s, 2);
				s+=2;
				x+=width*2+gap;
			}
			else
			{
				XmbDrawString(display, pixmap, fontset, gc, x, ascent, (char *)s, 1);
				s++;
				x+=width+gap;
			}
		}
		widthmax=x;
	}

	if(resultwidth)
		*resultwidth=widthmax;

	p=XGetImage(display, pixmap, 0, 0, maxWidth, maxHeight, 0xffffffff, ZPixmap);

	int i, j;
	unsigned long c;

	for(i=0; i < height; i++)
	{
		for(j=0; j < widthmax; j++)
		{
			c=XGetPixel(p, j, i);
			buffer[i][j]= 255 - c;
		}
	}


	XDestroyImage(p);

	unsigned char **buf;
	buf=new unsigned char * [height];
	for(i=0; i < height; i++)
		buf[i]=buffer[i];

	return buf;
}
