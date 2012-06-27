/*
 * x11fontset.cpp - x11 fontset management class (for avoiding memory leaks)
 *
 * $Id: x11fontset.cpp,v 1.4 2006/02/11 19:17:23 fumi Exp $
 *
 * Copyright (C) 2005, 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

#include "x11fontset.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <iterator>
#include <map>
using namespace std;

X11FontSet::X11FontSet(Display *d)
{
	display=d;
}

X11FontSet::~X11FontSet(void)
{

}

XFontSet X11FontSet::get(int height, int prop, bool latin)
{
	map<unsigned long, XFontSet>::iterator i;

	unsigned long key;
	key=height | (prop << 8) | (latin << 16);

	i=fontsets.find(key);
	if(i != fontsets.end())
		return i->second;

	XFontSet fontset;
	char **font_miss;
	int font_miss_num;
	char *font_def;

	char spc_character = ( prop ? 'p' : 'c' );

	char fontname[256];
	if(latin)
		sprintf(fontname, "-*-*-medium-r-normal--%d-*-*-*-%c-*-iso8859-1", height, spc_character);
	else
		sprintf(fontname, "\
-*-*-medium-r-normal--%d-*-*-*-%c-*-iso8859-1,\
-*-*-medium-r-normal--%d-*-*-*-%c-*-jisx0208.1983-0,\
-*-*-medium-r-normal--%d-*-*-*-%c-*-jisx0208.1990-0,\
-*-*-medium-r-normal--%d-*-*-*-%c-*-jisx0201.1976-0", height, spc_character, height, spc_character, height, spc_character, height, spc_character);


	fontset=XCreateFontSet(display, fontname, &font_miss, &font_miss_num, &font_def);



#ifdef DEBUG
	fprintf(stderr, "\
!!-------------- X11FontSet::get----------------------------------\n\
  XLFD %s\n\
", fontname);

	
	{
#if 0
		char **p;
		p=font_miss;
		while(*p != NULL)
			fprintf(stderr, "alternative font name %s\n", *p++);
#endif
	fprintf(stderr, "\
!!-----------------------------------------------------------------\n");
	fflush(stderr);
	}
#endif

	XFreeStringList(font_miss);

	fontsets.insert(pair<unsigned long, XFontSet>(key, fontset));

	return fontset;
}

