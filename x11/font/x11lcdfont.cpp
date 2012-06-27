/*
 * x11lcdfont.cpp - X11 font
 *
 * $Id: x11lcdfont.cpp,v 1.4 2006/02/11 19:17:23 fumi Exp $
 *
 * Copyright (C) 2004, 2005, 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//int SjisToEuc(unsigned char const *str, int len, unsigned char *out);

X11LcdFont::X11LcdFont(Display *_display, Window _window, int _width, int _height, int ascent, int gap, XFontSet fontset, int maxWidth, int maxHeight)
{
	display=_display;
	window=_window;
	width=_width;
	height=_height;

	this->ascent = ascent;

	this->maxWidth = maxWidth;
	this->maxHeight = maxHeight;

	buffer = new unsigned char * [maxHeight];
	int i;
	for( i = 0; i < maxHeight; i++ )
		buffer[ i ] = new unsigned char [maxWidth];

	this->fontset=fontset;
	this->gap=gap;

	int depth;
	depth=DefaultDepth(display, 0);
	pixmap=XCreatePixmap(display, window, maxWidth, maxHeight, depth);
	gc=XCreateGC(display, pixmap, 0, 0);

	black=BlackPixel(display, 0);
	white=WhitePixel(display, 0);
}

X11LcdFont::~X11LcdFont()
{
	XFreeGC(display, gc);
	XFreePixmap(display, pixmap);

	int i;
	for( i = 0; i < maxHeight; i++ )
		delete[] buffer[ i ];

	delete[] buffer;
}

unsigned char **X11LcdFont::drawString(char const *str, int *resultwidth)
{
	XImage *p;

	XSetForeground(display, gc, white);
	XFillRectangle(display, pixmap, gc, 0, 0, maxWidth, maxHeight);
	XSetForeground(display, gc, black);

	int len=strlen(str);
	int widthmax=len*(width+gap);
	if(gap == 0)
	{
		fprintf(stderr, "!! (%s)\n", str);
		fflush(stderr);

		XmbDrawString(display, pixmap, fontset, gc, 0, ascent, (char *)str, len);
	}
	else
	{
		int x=0;
		while(len--)
		{
			XmbDrawString(display, pixmap, fontset, gc, x, ascent, (char *)str, 1);
			x+=width+gap;
			str++;
		}
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

void X11LcdFont::freeBuffer(unsigned char **buf)
{
	delete[] buf;
}
