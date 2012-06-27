/*
 * timermgr.cpp - A timer management class
 *
 * $Id: timermgr.cpp,v 1.21 2006/02/11 19:17:22 fumi Exp $
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include "timer.h"
#include "timermgr.h"
#include "address.h"
#include "memory.h"
#include "state.h"
#include "ruputer.h"

#include "timerresourcemgr.h"
#include "timerresource.h"

TimerResourceMgr TimerMgr::timerResourceMgr;

TimerMgr::TimerMgr(Ruputer *ruputer, int n1hz, int n4hz, int n64hz)
{
	assert( ruputer );

	this->ruputer = ruputer;


	this->mem = new MemoryForInside();

	this->n1hz = n1hz;
	this->n4hz = n4hz;
	this->n64hz = n64hz;

	t1Hz  = new Timer *[n1hz];
	t4Hz  = new Timer *[n4hz];
	t64Hz = new Timer *[n64hz];

	memset(t1Hz, 0, sizeof(Timer *) * n1hz);
	memset(t4Hz, 0, sizeof(Timer *) * n4hz);
	memset(t64Hz, 0, sizeof(Timer *) * n64hz);
}

TimerMgr::~TimerMgr(void)
{
	closeAll();

	delete[] t1Hz;
	delete[] t4Hz;
	delete[] t64Hz;

	delete mem;
}

// Todo 優先順位の考慮
void TimerMgr::decrement1HzTimer(void)
{
	decrementTimer(n1hz, t1Hz);
}

void TimerMgr::decrement4HzTimer(void)
{
	decrementTimer(n4hz, t4Hz);
}
void TimerMgr::decrement64HzTimer(void)
{
	decrementTimer(n64hz, t64Hz);
}

void TimerMgr::decrementTimer(int n, Timer **timer)
{
	while( n-- )
	{
		if( *timer )
			(*timer)->decrement();

		timer++;
	}
}

unsigned long TimerMgr::createTimerCommon(int n, Timer **timer, unsigned long addr, unsigned short count)
{
	while( n-- )
	{
		if( *timer == NULL )
		{
			TimerResource *tr;

			tr = timerResourceMgr.allocTimerResource();

			*timer = createTimer(addr, count, tr);

			return tr->getTimerAddress();
		}
		timer++;
	}
	return 0;
}


unsigned long TimerMgr::create1HzTimer(unsigned long addr, unsigned short count)
{
	return createTimerCommon(n1hz, t1Hz, addr, count);
}

unsigned long TimerMgr::create4HzTimer(unsigned long addr, unsigned short count)
{
	return createTimerCommon(n4hz, t4Hz, addr, count);
}
unsigned long TimerMgr::create64HzTimer(unsigned long addr, unsigned short count)
{
	return createTimerCommon(n64hz, t64Hz, addr, count);
}


bool TimerMgr::subDeleteTimer(int n, Timer **timer, unsigned long addr)
{
	while( n-- )
	{
		if( *timer )
		{
			if( (*timer)->getTimerResource()->getTimerAddress() == addr )
			{
				timerResourceMgr.freeTimerResource( (*timer)->getTimerResource() );
				delete *timer;
				*timer = 0;

				return true;
			}
		}
		timer++;
	}

	return false;
}

void TimerMgr::deleteTimer(unsigned long addr)
{
	bool b;

	b = subDeleteTimer(n1hz, t1Hz, addr);
	if( b )
		return;
	b = subDeleteTimer(n4hz, t4Hz, addr);
	if( b )
		return;

	b = subDeleteTimer(n64hz, t64Hz, addr);
	if( b )
		return;

#ifdef DEBUG
		fprintf(stderr, "!! delete timer error %x\n", addr);
#endif
#ifdef DEBUG
	fprintf(stderr, "!! delete timer %x\n", addr);
#endif

}

Timer *TimerMgr::createTimer(unsigned long addr, unsigned short count, TimerResource *tr)
{
	Timer *t;

	t = new Timer(ruputer, addr, count, tr);

	return t;
}


void TimerMgr::subCloseAll(int n, Timer **timer)
{
	while( n-- )
	{
		if( *timer )
		{
			timerResourceMgr.freeTimerResource( (*timer)->getTimerResource() );
			delete *timer;
			*timer = 0;
		}
		timer++;
	}
}
/** 全ての登録されているタイマを閉じる
 *
 */
void TimerMgr::closeAll(void)
{
	subCloseAll(n1hz, t1Hz);
	subCloseAll(n4hz, t4Hz);
	subCloseAll(n64hz, t64Hz);

#if 0
	delete t1Hz[0];
	t1Hz[0]=NULL;
	delete t1Hz[1];
	t1Hz[1]=NULL;
	delete t4Hz;
	t4Hz=NULL;
	delete t64Hz;
	t64Hz=NULL;
#endif
}
