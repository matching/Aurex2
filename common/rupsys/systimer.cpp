/*
 * systimer.cpp - Rupsys timer emulation
 *
 * $Id: systimer.cpp,v 1.7 2004/10/31 13:41:18 fumi Exp $
 *
 * Copyright (C) 2004 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

#include "rupaddress.h"
#include "systimer.h"
#include "ruputer.h"
#include "keybuffer.h"

SysTimer::SysTimer(RupAddress *systemAddress, KeyBuffer *keybuffer)
{
	this->systemAddress = systemAddress;
	this->keybuffer = keybuffer;

	assert( systemAddress );
	assert( keybuffer );

	mem=new MemoryForInside(A_TDATA);
}
SysTimer::~SysTimer(void)
{
	delete mem;
}


int SysTimer::readHour(void)
{
	return mem->read16_u(0);
}

void SysTimer::writeHour(int hour)
{
	mem->write16(0, hour);
}

int SysTimer::readMin(void)
{
	return mem->read16_u(2);
}

void SysTimer::writeMin(int min)
{
	mem->write16(2, min);
}


int SysTimer::readSec(void)
{
	return mem->read16_u(4);
}

void SysTimer::writeSec(int sec)
{
	mem->write16(4, sec);
}


int SysTimer::readCo(void)
{
	return mem->read16_u(6);
}

void SysTimer::writeCo(int co)
{
	mem->write16(6, co);
}


void SysTimer::decrement(void)
{
	int co, sec, min, hour;

#ifdef DEBUG
	fprintf(stderr, "!! systimer decrement !! %d\n", systemAddress->get_timerstat());
#endif

	if(systemAddress->get_timerstat() == STS_RUN)
	{
		co=readCo();
		co--;
		if(co >= 0)
			writeCo(co);
		else
		{
/* 毎秒イベントチェック */
#ifdef DEBUG
			fprintf(stderr, "!! systimer 1sec decrement .... %x\n", systemAddress->get_eventrequest());

#endif


			if(systemAddress->get_eventrequest() & EVENT_TMDWN2)
			{
				keybuffer->bsetFirstItem(0x8000);
				systemAddress->bset_event(EVENT_TMDWN2);
			}

			min=readMin();
			hour=readHour();
			sec=readSec();

			if(min == 0	&& hour == 0 && sec <= 5)
			{
				if(systemAddress->get_eventrequest() & EVENT_TMDWN0)
				{
					keybuffer->bsetFirstItem(0x8000);
					systemAddress->bset_event(EVENT_TMDWN0);
				}
			}

			writeCo(3);
			sec--;
			if(sec >= 0)
				writeSec(sec);
			else
			{
				writeSec(59);
				min--;
				if(min >= 0)
					writeMin(min);
				else
				{
					writeMin(59);
					hour--;
					if(hour >= 0)
						hour--;
					else
					{
				// ボローしたら、hour がマイナス→タイムアップ
						if(systemAddress->get_eventrequest() & EVENT_TMDWN1)
						{
							keybuffer->bsetFirstItem(0x8000);
							systemAddress->bset_event(EVENT_TMDWN1);
						}
						systemAddress->set_timerstat(STS_STOP);

						writeCo(0);
						writeSec(0);
						writeMin(0);
						writeHour(0);
					}
				}
			}
		}
	}
}
