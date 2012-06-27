/*
 * timer.cpp - Timer
 *
 * $Id: timer.cpp,v 1.17 2006/02/17 19:01:38 fumi Exp $
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

#include <assert.h>

#include "timer.h"
#include "ruputer.h"
#include "memory.h"
#include "thread.h"
#include "state.h"

#include "timerresource.h"

#include <list>
using namespace std;

Timer::Timer(Ruputer *ruputer, unsigned long func_addr, int count, TimerResource *tr)
{
	this->ruputer = ruputer;
	this->threadList = ruputer->getThreadList();

	this->func_addr = func_addr;
	this->count = count;
	this->stack_pointer = tr->getStackPointer();
	this->executingFlag = 0;
	this->kernelsp = tr->getKernelStackPointer();
	this->tr = tr;

	assert( ruputer );
	assert( tr );

	timeraddr = new MemoryForInside( tr->getTimerAddress() );
	timeraddr->write16(0, count);
}

Timer::~Timer(void)
{
	delete timeraddr;
}

TimerResource *Timer::getTimerResource(void)
{
	return tr;
}

void Timer::decrement(void)
{
	unsigned short c;

	c=timeraddr->read16_u(0);
	if(c == 1)
	{
#if 1
		if(createThread() == 1)
			return;
#else
		createThread();
#endif

		c=count;
	}
	else if(c > 0)
		c--;

#ifdef DEBUG
	fprintf(stderr, "!! write timer address %d\n", c);
#endif

	timeraddr->write16(0, c);
}



int Timer::createThread(void)
{
#ifdef DEBUG
	fprintf(stderr, "## timer thread attenpt to create...  %d\n", executingFlag);
#endif

	if(executingFlag == 1)
		return 1;
	else
	{
		executingFlag=1;

		Thread *t = new Thread(func_addr, stack_pointer, ruputer, kernelsp, this);
		t->getState()->setState( ThreadState::RUNNING );
		threadList->push_front(t);

		return 0;
	}



#if 0
	Thread *t;

#ifdef DEBUG
	fprintf(stderr, "## timer thread attenpt to create...  %d\n", executingFlag);
#endif

	if(executingFlag == 1)
		return 1;
	else
	{
		executingFlag=1;

#ifdef DEBUG
		fprintf(stderr, "!!create timer tharead pc %lx, sp %lx\n", func_addr, stack_pointer);
		fflush(stderr);
#endif
		t=parent->fork(func_addr, stack_pointer, kernelsp, this);
		t->state->setState(STATE_RUN);
//		::ruputer->threadmgr->add(t);
		::ruputer->threadmgr.push_front(t);

#ifdef DEBUG
		fprintf(stderr, "!!created timer tharead\n");
		fflush(stderr);
#endif

	}
	return 0;
#endif

}
