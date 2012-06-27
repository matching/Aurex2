/*
 * thread.cpp - A unit of Ruputer emulation
 *
 * $Id: thread.cpp,v 1.37 2008/07/26 06:43:08 fumi Exp $
 *
 * Copyright (C) 2004 2005, 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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
#include <assert.h>
#include <string.h>

#include "address.h"
#include "rupaddress.h"
#include "cpu.h"
#include "timer.h"
#include "state.h"
#include "interruption.h"
#include "ruputer.h"
#include "thread.h"
#include "memory.h"
#include "keybuffer.h"
#include "ruputerdirectory.h"
#include "directoryconverter.h"

#include "psdos.h"
#include "lcd.h"
#include "wbios.h"
#include "rupsys.h"

Thread::Thread(unsigned long pc, unsigned long a3, Ruputer *ruputer, unsigned long kernelsp, Timer *trigger, ThreadState *state)
{
	initial_pc = pc;
	initial_a3 = a3;
	initial_kernelsp = kernelsp;
	this->trigger = trigger;

	if( state )
	{
		this->state = state;
//		assert( isMainThread() );
		mainThread = true;
	}
	else
	{
		this->state = new ThreadState();
		mainThread = false;
	}

	this->cpu = new Cpu(pc, a3,
						new Interruption(new DosIntr(ruputer, this),
										 new LcdIntr(ruputer),
										 new BiosIntr(ruputer, this),
										 new SysIntr(ruputer, this, kernelsp)),
						ruputer->getMemoryForCpu());
	this->keybuffer = ruputer->getKeyBuffer();

	assert( keybuffer );
}

Thread::~Thread(void)
{
	if(trigger)
		trigger->executingFlag=0;

	delete cpu;

	if( ! mainThread )
		delete state;
}

int Thread::isStartFunctionOver(void)
{
	if(cpu->regs.pc == initial_pc && cpu->regs.a[3] == initial_a3)
		return 1;

	if(state->getState() == ThreadState::EXIT)
		return 2;
	return 0;
}

bool Thread::execute(int *cycle)
{
#ifdef DEBUG_THREAD
	fprintf(stderr, "!!Thread::execute %p / pc:0x%08x sp 0x%08x state:%d cyc %d key state %x nest level %d evntReq %x\n",
			this,
			cpu->regs.pc, cpu->regs.a[3],
			state->getState(), *cycle, -1,
			*(unsigned long *)Memory::getMemoryPointer(A_KERNEL_MAIN_SP), -1);
	fflush(stderr);
#endif

	int ret;

	switch(state->getState())
	{
	case ThreadState::WAIT_KEYIN:
		ret=keybuffer->emu_Keyin(&(cpu->regs));
		if(ret != 0)
			state->setState( ThreadState::RUNNING);
		break;
	case ThreadState::WAIT_BI:
		ret=keybuffer->emu_bi_getbtn(&(cpu->regs));
		if(ret != 0)
			state->setState( ThreadState::RUNNING);
		break;

	case ThreadState::CHILD_RUNNING:
		break;

	case ThreadState::EXIT:
		return false;

	case ThreadState::RUNNING:
		break;
	}

	if(*cycle < 0)
		return true;

	cpu->setCycle(*cycle);
	while(state->getState() == ThreadState::RUNNING)
	{
		ret=cpu->emulate();

		if(isStartFunctionOver())
		{
			*cycle=ret;
			return false;
		}

		if(ret <= 0)
			break;
	}

	*cycle=cpu->getCycle();
	return true;
}

void Thread::stop(void)
{
	cpu->setCycle(-1);
}

ThreadState *Thread::getState(void)
{
	return state;
}


bool Thread::isMainThread(void)
{
	return false;
}

unsigned long Thread::getStackPointer(void)
{
	return cpu->regs.a[2];
}

unsigned long Thread::getExitCode(void)
{
	return cpu->regs.d[0];
}

MainThread::MainThread(unsigned long pc, unsigned long a3, Ruputer *ruputer, unsigned long kernelsp, ThreadState *state) : Thread(pc, a3, ruputer, kernelsp, 0 /* not trigger */, state)
{
}



MainThread::~MainThread(void)
{
#ifdef DEBUG
	fprintf(stderr, "!! main thread dest\n");
	fflush(stderr);
#endif

// メインスレッドだけの特殊な処理
// ex. Aurex のステータスも左右するようなフラグの設定
//	ruputerState->setState(STATE_INIT);
	state->setState( ThreadState::EXIT );
}

bool MainThread::isMainThread(void)
{
	return true;
}


ChildThread::ChildThread(unsigned long pc, unsigned long a3, Ruputer *ruputer, unsigned long kernelsp ) : Thread(pc, a3, ruputer, kernelsp, 0 /* not trigger */, 0 )
{
}



ChildThread::~ChildThread(void)
{
}

bool ChildThread::isMainThread(void)
{
	return true; /* main thread と同じ扱い→dos_exit で終了しないようにする */
}


