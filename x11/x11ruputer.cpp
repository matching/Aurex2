/*
 * x11ruputer.cpp - X11 library ruputer emulation
 *
 * $Id: x11ruputer.cpp,v 1.15 2006/03/06 17:09:43 fumi Exp $
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


#include "keybuffer.h"
#include "state.h"
#include "rupaddress.h"
#include "cmlcd.h"

#include "unixfindfile.h"

#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/Xlocale.h>

#include "x11ruputer.h"
#include "x11exception.h"
#include "x11lcd.h"
#include "x11fontset.h"

// todo ?
#define NOT_SUPPORT_SOUND (NULL)

static X11Ruputer *x11ruputer;

X11Ruputer::X11Ruputer(int argc, char **argv, Frame *_frame, Directory *_sddir, Option *option, UFilerIni *uini) : Ruputer(_frame, _sddir,  option, uini)
{
	x11ruputer = this;

	CMLcd *cmlcd;

	cmlcd = getCMLcd();

	unsigned long background;
	unsigned long foreground;

	setlocale(LC_ALL, "");

	display = XOpenDisplay(NULL);
	if(display == NULL)
		throw X11DisplayOpenException();

	background=WhitePixel(display, 0);
	foreground=BlackPixel(display, 0);

	window = XCreateSimpleWindow(display,
								 DefaultRootWindow(display),
								 0, 0, // x, y
								 cmlcd->getRealWidth102(), // width
								 cmlcd->getRealHeight64(), // height
								 0, // border width
								 0, // depth
								 background);

	XSetStandardProperties(display,
						   window,
						   "Aurex", "Aurex",
						   None, argv, argc, NULL);



//	XMapWindow(display, window);
	x11fontset = new X11FontSet(display);
	x11lcd = new X11Lcd(display, window, cmlcd, x11fontset, getSystemAddress());
	setSDLcd(x11lcd);

	XMapRaised(display,window);
	XSelectInput(display, window, KeyPressMask | KeyReleaseMask | ExposureMask);
//	XSelectInput(display, window, KeyReleaseMask | ExposureMask);
}

X11Ruputer::~X11Ruputer(void)
{
	delete x11fontset;
	delete x11lcd;
}


int X11Ruputer::EventProcedure(void)
{
	XEvent event;
	Bool ret;
	KeySym key=0;

	ret=XCheckMaskEvent(display, KeyPressMask | KeyReleaseMask | ExposureMask, &event);
	if(ret == False)
		return 0;

	switch(event.type)
	{
	case KeyPress:
	{
		key=XLookupKeysym(&event.xkey, 0);

		switch(key)
		{
		case XK_Tab:
			requestReload();
			break;

		case XK_Left:
			keybuf->enQueueKey(Bkey_lf | KEY_PUSH_DOWN);
			break;
		case XK_Down:
			keybuf->enQueueKey(Bkey_dw | KEY_PUSH_DOWN);
			break;
		case XK_Up:
			keybuf->enQueueKey(Bkey_up | KEY_PUSH_DOWN);
			break;
		case XK_Right:
			keybuf->enQueueKey(Bkey_rg | KEY_PUSH_DOWN);
			break;
		case 'z':
			keybuf->enQueueKey(Bkey_A | KEY_PUSH_DOWN);
			break;
		case 'w':
			keybuf->enQueueKey(Bkey_B | KEY_PUSH_DOWN);
			break;
		case 's':
			keybuf->enQueueKey(Bkey_C | KEY_PUSH_DOWN);
			break;
		case 'x':
			keybuf->enQueueKey(Bkey_D | KEY_PUSH_DOWN);
			break;
		case 'q':
			requestDestroy();
		break;
		}
		break;
	}
	case KeyRelease:
		key=XLookupKeysym(&event.xkey, 0);
		switch(key)
		{
		case XK_Left:
			keybuf->enQueueKey(Bkey_lf | KEY_PUSH_UP);
			break;
		case XK_Down:
			keybuf->enQueueKey(Bkey_dw | KEY_PUSH_UP);
			break;
		case XK_Up:
			keybuf->enQueueKey(Bkey_up | KEY_PUSH_UP);
			break;
		case XK_Right:
			keybuf->enQueueKey(Bkey_rg | KEY_PUSH_UP);
			break;
		case 'z':
			keybuf->enQueueKey(Bkey_A | KEY_PUSH_UP);
			break;
		case 'w':
			keybuf->enQueueKey(Bkey_B | KEY_PUSH_UP);
			break;
		case 's':
			keybuf->enQueueKey(Bkey_C | KEY_PUSH_UP);
			break;
		case 'x':
			keybuf->enQueueKey(Bkey_D | KEY_PUSH_UP);
			break;
		}
		break;
	}

	return 1;
}


int X11Ruputer::load(char const *filename) throw()
{
	try{
		Ruputer::load(filename);
	}
	catch( ExfFileInvalidException &e ) {
		fprintf(stderr, "exf file is invalid (%s)\n", filename);
		return 1;
	}
	catch( ExfFileNotFoundException &e ) {
		fprintf(stderr, "A exf file associated with (%s) is not found\n", filename);
		return 1;
	}
	catch( AssociatedFileNotInVirtualDirectoryException &e ) {
		fprintf(stderr, "A file (%s) is out of virtual directory.\n", filename);
		return 1;
	}

	return 0;
}


SDFindFile *X11Ruputer::createSDFindFile(struct dta_t *dta_area, char const *pathname, unsigned char search_attribute)
{
	return new UnixFindFile(dta_area, pathname, search_attribute);
}
