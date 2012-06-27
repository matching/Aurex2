/*
 * rupstruct.cpp - Ruputer (system) struct management module
 *
 * $Id: rupstruct.cpp,v 1.6 2004/02/14 05:24:43 fumi Exp $
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
#include <time.h>

#include "rupstruct.h"
#include "memory.h"
#include "address.h"
#include "ruputer.h"

WatchDT::WatchDT(void)
{
	mem=new MemoryForInside(A_WATCHDT);
	updateWatchDT();
}

WatchDT::~WatchDT(void)
{
	delete mem;
}

void WatchDT::updateWatchDT(void)
{
	time_t t;

	t=time(NULL);
	local=localtime(&t);

	mem->write16(0, local->tm_year+1900);
	mem->write16(2, local->tm_mon+1);
	mem->write16(4, local->tm_mday);
	mem->write16(6, local->tm_wday);
	mem->write16(8, local->tm_hour);
	mem->write16(10, local->tm_min);
	mem->write16(12, local->tm_sec);
}


int WatchDT::getYear(void)
{
	return local->tm_year+1900;
}
int WatchDT::getMonth(void)
{
	return local->tm_mon+1;
}

int WatchDT::getMDay(void)
{
	return local->tm_mday;
}
int WatchDT::getWDay(void)
{
	return local->tm_wday;
}

int WatchDT::getHour(void)
{
	return local->tm_hour;
}
int WatchDT::getMin(void)
{
	return local->tm_min;
}
int WatchDT::getSec(void)
{
	return local->tm_sec;
}

int WatchDT::getYearBcd(void)
{
	return toBcd(getYear());
}

int WatchDT::getMonthBcd(void)
{
	return toBcd(getMonth());
}

int WatchDT::getMDayBcd(void)
{
	return toBcd(getMDay());
}

int WatchDT::getHourBcd(void)
{
	return toBcd(getHour());
}

int WatchDT::getMinBcd(void)
{
	return toBcd(getMin());
}

int WatchDT::getSecBcd(void)
{
	return toBcd(getSec());
}

/************* to 二進化十進数 ***************************/
int WatchDT::toBcd(int num)
{
	int i;
	unsigned long c, res;

	res=0;
	for(i=0; i < 24/4; i++)
	{
		c=(num % 10);
		num/=10;
		res|=c << (i*4);
	}

	return (int)res;
}
