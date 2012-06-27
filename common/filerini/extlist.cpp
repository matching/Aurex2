/*
 * extlist.cpp - ext list management class
 *
 * $Id: extlist.cpp,v 1.3 2005/07/27 02:34:32 fumi Exp $
 *
 * Copyright (C) 2004, 2005, 2012 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

#include <string.h>

#include "extlist.h"

#ifdef DEBUG
#include <stdio.h>
#endif

#include <set>
#include <string>
#include <iterator>
using namespace std;

ExtList::ExtList(void)
{
}

ExtList::~ExtList(void)
{
}


void ExtList::add(char const *ext)
{
#ifdef DEBUG
	fprintf(stderr, "ExtList::add (%s)\n", ext);
	fflush(stderr);
#endif

	exts.insert( string(ext) );
}

void ExtList::add(ExtList *extlist)
{
	if( extlist == NULL )
		return;

	exts.insert( extlist->exts.begin(), extlist->exts.end() );
}

void ExtList::getExtListString(char *extlist)
{
	*extlist = 0;

	set<string>::iterator i;

	i = exts.begin();
	for(; i != exts.end(); i++)
	{
#ifdef DEBUG
		fprintf(stderr, "ExtList::getExtListString add result (%s)\n", (*i).c_str());
		fflush(stderr);
#endif

		strcat(extlist, (*i).c_str());
		strcat(extlist, ",");
	}
}
