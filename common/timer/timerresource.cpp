/*
 * timerresource.cpp - Timer Resource ( base address, table address, stack pointer, kernel stack pointer ) keeping class
 *
 * $Id: timerresource.cpp,v 1.3 2006/02/16 13:05:13 fumi Exp $
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

#include "timerresource.h"


TimerResource::TimerResource(unsigned long baseAddr)
{
	this->baseAddr = baseAddr;
}

TimerResource::~TimerResource(void)
{
}

unsigned long TimerResource::getBaseAddress(void)
{
	return baseAddr;
}
unsigned long TimerResource::getTimerAddress(void)
{
	return baseAddr - 0x2;
}
unsigned long TimerResource::getStackPointer(void)
{
	return baseAddr - 0x4;
}

unsigned long TimerResource::getKernelStackPointer(void)
{
	return baseAddr - 0x400;
}

