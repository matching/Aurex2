/*
 * dirstacksjis.cpp - Directory string parser module for (Japanese) Shift JIS
 *
 * $Id: dirstacksjis.cpp,v 1.3 2006/02/16 18:19:40 fumi Exp $
 *
 * Copyright (C) 2005 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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
#include <string.h>
#include <assert.h>

#include "cessjis.h"
#include "dirstacksjis.h"
#include "directoryelement.h"


DirStackSjis::DirStackSjis(void)
{
}

DirStackSjis::DirStackSjis(char const *dir, char dirsep) : DirStack()
{
	head.child=head.parent=&head;
	makeDirStack(dir, dirsep);
}

DirStackSjis::~DirStackSjis(void)
{
}

/**
 * Shift_JIS を考慮した makeDirStack の実装
 */
void DirStackSjis::makeDirStack(char const *dir, char dirsep)
{
	int state;
	state=0;
	char const *start = NULL;
	int n;
	CesSjis sjis;

	while(*dir)
	{
		n = sjis.checkChar(dir);

		if( n == 2 )
		{
			if( state == 0 )
			{
				start=dir;
				state = 1;
			}
		}
		else
		{
			switch(state)
			{
			case 0:
				if(*dir != dirsep)
				{
					start=dir;
					state=1;
				}
				break;
			case 1:
				if(*dir == dirsep)
				{
					push(start, dir);
					state=0;
				}
				break;
			}
		}
		dir += n;
	}

	switch(state)
	{
	case 0:
		break; // do nothing
	case 1:
		push(start, dir);
	}
}
