/*
 * x11lcd.cpp - x11 graphic class
 *
 * $Id: x11lcd.cpp,v 1.13 2006/02/16 18:19:41 fumi Exp $
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
#include "cmlcdfunc.h"
#include "rupaddress.h"

#include <stdio.h>


X11Lcd::X11Lcd(Display *_display, Window _window, CMLcd *cmlcd, X11FontSet *fontsets, RupAddress *systemAddress) : SDLcd(cmlcd)
{
	this->systemAddress = systemAddress;
	display=_display;
	window=_window;
	x11fontsets=fontsets;

	gc=XCreateGC(display, window, 0, 0);

	int format, bitmap_pad;
	depth=DefaultDepth(display, 0);
	format=ZPixmap;
	bitmap_pad=32;

    ximage = XCreateImage(display, DefaultVisual(display, 0),
						  depth, format, 0, 0,
						  cmlcd->getRealWidth102(),
						  cmlcd->getRealHeight64(),
						  bitmap_pad, 0);

	ximage->data=new char[ximage->bytes_per_line * cmlcd->getRealHeight64()];
}

X11Lcd::~X11Lcd()
{
	delete[] ximage->data;
	ximage->data = NULL;

	XDestroyImage(ximage);
	XFreeGC(display, gc);
}

void X11Lcd::updateWindow(void)
{
	int x, y;
	unsigned char const *p;
	int width, height;


	height = cmlcd->getRealHeight64();
	width = cmlcd->getRealWidth102();

	for(y=0; y < height; y++)
	{
		p = cmlcd->getCMLcdFunc()->getPixelArray(0, y);

		for(x=0; x < width; x++)
		{
			unsigned char c;
			unsigned long color;

			c = *(p + x);
			c = 255 - c;
			switch(depth)
			{
			case 32:
				color=((unsigned long)c << 24) | (c << 3+10) | (c << 2); // 当ってるのか？
				break;
			default:
			case 24:
				color=((unsigned long)c << 16) | (c << 8) | c;
				break;
			case 16:
				color=((unsigned long)(c & 0xf8) << 8) | ((c & 0xfc) << 3) | (c >> 3);
				break;
			case 8:
				color=(c & 0xc0) | ((c & 0xe0) >> 2) | (c >> 5);
				break;
			}
			XPutPixel(ximage, x, y, color);

	//				XSetForeground(display, gc, ((unsigned long)c << 16) | (c << 8) | c);
	//				XDrawPoint(display, window, gc, x, y);
		}
	}

	XPutImage(display, window, gc, ximage, 0, 0, 0, 0, cmlcd->getRealWidth102(), cmlcd->getRealHeight64());

}


SDLcdFont *X11Lcd::createSDLcdFont(int width, int height, int prop, int gap, bool latin)
{
	XFontSet fontset;
	fontset=x11fontsets->get(height, prop, latin);

#ifdef DEBUG
	fprintf(stderr, "X11Lcd::createSDLcdFont height:%d, prop:%d, latin:%d => %p\n", height, prop, latin, fontset);
	fflush(stderr);
#endif

	if( fontset == NULL )
		return NULL;

	int ascent;
	XFontSetExtents *fontsetExtents;

	fontsetExtents = XExtentsOfFontSet( fontset );

	ascent = -fontsetExtents->max_logical_extent.y;
	ascent--;

	if(latin)
		return new X11LcdFont(display, window, width, height, ascent, gap, fontset, cmlcd->getRealWidth320(), 32 * cmlcd->getExpansion());
	else
		return new X11LcdFontSjis(display, window, width, height, ascent, gap, fontset, cmlcd->getRealWidth320(), 32 * cmlcd->getExpansion());

#ifdef DEBUG
	fprintf(stderr, "kokokuru?\n");
	fflush(stderr);
#endif

}

SDLcdFont *X11Lcd::createSDLcdFontAa(int width, int height, int prop, int gap, bool latin)
{
	return createSDLcdFont(width, height, prop, gap, latin);
}

bool X11Lcd::hasFontAa(void)
{
	return true;
}
