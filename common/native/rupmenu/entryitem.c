/*
 * entryiteme.c - rupmenu emulation code (related other function) (MN10200 code)
 *
 * $Id: entryitem.c,v 1.7 2006/01/21 15:39:56 fumi Exp $
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

#include <string.h>
#include <stdlib.h>

#include "rupmenucommon.h"


int sub_entryItem(char *str, char *list, int num)
{
	INPUTITEM *currentInputItem;

	currentInputItem = currentPlane->inputItems + currentPlane->inputItemIndex;

	memset(currentInputItem, 0, sizeof( INPUTITEM ) );

	currentInputItem->myid = getMenuId();
	currentInputItem->myname = str;
	currentInputItem->listlen = strlen(list);
	currentInputItem->chmode = 0; /* input */
	currentInputItem->list = list;
	currentInputItem->listtgl = 1; /* default ( toggle ) */
	currentInputItem->listnum = num;

	currentPlane->inputItemIndex++;

/*
	emu_gv_kput(0, 0, str, 0, 0, 0);
	emu_gv_kput(0, 13, list, 0, 0, 0);
*/

	return currentInputItem->myid;
}

int sub_entryItemEx(char *str, int (*func)(int, int, int))
{
	INPUTITEM *currentInputItem;

	currentInputItem = currentPlane->inputItems + currentPlane->inputItemIndex;

	memset(currentInputItem, 0, sizeof( INPUTITEM ) );

	currentInputItem->myid = getMenuId();
	currentInputItem->myname = str;
	currentInputItem->chmode = 2; /* inputex */
	currentInputItem->listEx = func;

	currentPlane->inputItemIndex++;

	return currentInputItem->myid;
}


int sub_entryItemMenu(char *str, int (*menufunc)(void))
{
	INPUTITEM *currentInputItem;

	currentInputItem = currentPlane->inputItems + currentPlane->inputItemIndex;

	memset(currentInputItem, 0, sizeof( INPUTITEM ) );

	currentInputItem->myid = getMenuId();
	currentInputItem->myname = str;
	currentInputItem->chmode = 1; /* plainmenu */
	currentInputItem->menu = menufunc;

	currentPlane->inputItemIndex++;

	return currentInputItem->myid;
}

int  sub_setInitialItemId(int id)
{
// ???
	return 0;
}

void sub_setInputFont(int id)
{
	currentPlane->font = id;
}


void sub_setChfiler(int flg)
{
// do nothing for emulating
}


int sub_setItemListToggle(int id, int toggle)
{
	INPUTITEM *p;

	p = getInputItem( id );
	if ( p == NULL )
		return -1;

	p->listtgl = toggle;

	return 0;
}


int sub_setItemListVal(int id, int val)
{
	INPUTITEM *p;

	p = getInputItem( id );
	if ( p == NULL )
		return -1;

	p->listval = val;
	return 0;
}


int sub_getItemListVal(int id)
{
	INPUTITEM *p;

	p = getInputItem( id );
	if ( p == NULL )
		return -1;

	return p->listval;
}

char *sub_getItemListStr(int id, int val)
{
	INPUTITEM *p;

	p = getInputItem( id );
	if ( p == NULL )
		return (char *)-1;

/*	if ( p->chmode = CHMODE_Input )
 *  ??????
 */
	return (char *)get_input_item_string_pointer(p, val);
}


INPUTITEM *sub_getInputItemNow(void)
{
	return currentPlane->inputItems + currentPlane->select;
}

int sub_getItemId(char *str)
{
	return searchStringPointer( str );
}

int sub_getLastItemId(void)
{
	return currentPlane->inputItems[currentPlane->select].myid;
}

int sub_getLastKeyState(void)
{
// ?????
	return last_key_code;
}

void sub_arrangeItemListX(int id0, int id1)
{
// あとでつくるかも
}

int sub_setItemMenuNext(int id, int flg)
{
	INPUTITEM *p;

	p = getInputItem( id );
	if ( p == NULL )
		return -1;

	p->nextflg = flg;
	return 0;
}


int sub_setItemSelectMode(int id, int val)
{
	INPUTITEM *p;

	p = getInputItem( id );
	if ( p == NULL )
		return -1;

	p->dspmode = val;
	return 0;
}


int sub_setItemDisable(int id, int val)
{
	INPUTITEM *p;

	p = getInputItem( id );
	if ( p == NULL )
		return -1;

	p->disable = val;
	return 0;
}


int sub_setItemCallBack(int id, void (*func)(int key, int val))
{
	return 0;
}

void sub_dispItemRW(int id)
{
// ??????
}


int sub_retToggle(int val, int max)
{
	if( val > max )
		return 0;
	if( val < 0 )
		return max;
	return val;
}

int sub_retToggleBoth(int val, int min, int max)
{
	if( val > max )
		return min;
	if( val < min )
		return max;
	return val;
}

int sub_retNonToggle(int val, int max)
{
	if( val > max )
		return max;
	if( val < 0 )
		return 0;

	return val;
}


int sub_retNonToggleBoth(int val, int min, int max)
{
	if( val > max )
		return max;
	if( val < min )
		return min;
	return val;
}

#if 0
int sub_entryItemTime(char *str,INPUTTIME *tm)
{
}


void setDblSignal(int flg);
void resetKeyflg(void);

#endif
