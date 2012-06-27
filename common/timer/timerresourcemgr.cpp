/*
 * timerresourcemgr.cpp - Timer resource management class
 *
 * $Id: timerresourcemgr.cpp,v 1.2 2005/01/10 08:40:28 fumi Exp $
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
#include <stdlib.h>
#include <string.h>

#include "timerresourcemgr.h"
#include "timerresource.h"

#define USED 1
#define NOT_USED 0

TimerResourceMgr::TimerResourceMgr(void)
{
	memset(used, 0, sizeof(used));
}

TimerResourceMgr::~TimerResourceMgr(void)
{
}

TimerResource *TimerResourceMgr::allocTimerResource(void)
{
	int i;

	i = getFreeIndex();
	if( i == -1 )
		return NULL;

	used[i] = USED;

	return new TimerResource( indexToBaseAddress( i ) );
}

void TimerResourceMgr::freeTimerResource(TimerResource *res)
{
	int i;

	i = baseAddressToIndex( res->getBaseAddress() );
	used[i] = NOT_USED;

	delete res;
}

unsigned long TimerResourceMgr::indexToBaseAddress(int index)
{
	return 0x21000 + 0x1000 * index;
}

int TimerResourceMgr::baseAddressToIndex(unsigned long addr)
{
	return ( addr - 0x21000 ) / 0x1000;
}


int TimerResourceMgr::getFreeIndex(void)
{
	int i;

	for( i = 0; i < 16; i++ )
	{
		if( used[i] == NOT_USED )
			return i;
	}

	return -1;
}
