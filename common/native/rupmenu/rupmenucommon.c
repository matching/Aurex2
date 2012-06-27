/*
 * rupmenucommon.c - rupmenu emulation code (related utility functions) (MN10200 code)
 *
 * $Id: rupmenucommon.c,v 1.6 2006/01/21 15:39:56 fumi Exp $
 *
 * Copyright (C) 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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


#include "rupmenucommon.h"

#if 1
#include "itoa.h"
#include <string.h>
#include "emu_lcdbios.h"
#endif

struct RupmenuPlane menuPlane[MAX_INPUT_ITEM];
struct RupmenuPlane *currentPlane;
int menuPlaneDepth = 0;
int last_key_code = 0;




int getMenuId(void)
{
	return ( (menuPlaneDepth-1) << 8 ) + currentPlane->inputItemIndex;
}

INPUTITEM *getInputItem(int id)
{
	int depth;
	int index;

	depth = (id >> 8);
	index = id & 0xff;

#if 0
	{
		char str[100] = "getInputItem, depth (";
		
		util_uitoa_supply_char(depth, str + strlen(str), 3, '0');
		strcat(str, "), index (");
		util_uitoa_supply_char(index, str + strlen(str), 3, '0');
		strcat(str, "");
		emu_gv_kput(300, 100, str, 0, 0, 0);
	}
#endif


	if ( depth > menuPlaneDepth )
		return 0;

	if ( index > menuPlane[ depth ].inputItemIndex )
		return 0;

	return menuPlane[ depth ].inputItems + index ;
}


int searchStringPointer(char *myname)
{
	int i, j;
	int end;

/* カレントが怪しいので先に行なう */
/*
	end = currentPlane->inputItemIndex;
	for( j = 0; i < end; j++ )
	{
		if ( currentPlane->inputItems[ i ].myname == myname )
			return currentPlane->inputItems[ i ].myid;
	}
*/
	for( i = 0; i < menuPlaneDepth; i++ )
	{
		end = menuPlane[i].inputItemIndex;
		for( j = 0; j < end; j++ )
		{
			if ( menuPlane[i].inputItems[ j ].myname == myname )
				return menuPlane[i].inputItems[ j ].myid;
		}
	}
	return -1;
}


char const *get_input_item_string_pointer(INPUTITEM *localInput, int no)
{
#if 0
	{
		char str[100] = "get_input_item_string_pointer, No (";
		
		util_uitoa_supply_char(no, str + strlen(str), 3, '0');
		strcat(str, "), listlen (");
		util_uitoa_supply_char(localInput->listlen, str + strlen(str), 3, '0');
		strcat(str, "");		
		emu_gv_kput(300, 100, str, 0, 0, 0);
	}
#endif

	return localInput->list + (localInput->listlen + 1) * no;
}
