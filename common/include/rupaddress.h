// -*-c++-*-
// $Id: rupaddress.h,v 1.11 2004/10/31 13:41:17 fumi Exp $

#ifndef RUPADDRESS_H
#define RUPADDRESS_H

#include <stdio.h>
#include "address.h"
#include "memory.h"


#define	EVENT_SEC		0x00040000UL	/* 秒毎イベントの要求/発生						*/
#define	EVENT_TMDWN0	0x01000000UL	/* タイムアップ予告音							*/
#define	EVENT_TMDWN1	0x02000000UL	/* タイムアップ音								*/
#define	EVENT_TMDWN2	0x04000000UL	/* タイムアップ(1秒)					  	    */


class RupAddress
{
	MemoryForInside *mem;

  public:

	RupAddress(void)
	{
		mem=new MemoryForInside();
	}

	virtual ~RupAddress(void)
	{
		delete mem;
	}

	unsigned char get_eventregister(void)
	{
		return mem->read8_u(A_EVENTREGISTER);
	}

	void set_eventregister(unsigned char dat)
	{
		mem->write8(A_EVENTREGISTER, dat);
	}


	unsigned long get_eventrequest(void)
	{
		return mem->read32(A_EVENTREQ);
	}
	void set_eventrequest(unsigned long dat)
	{
		mem->write32(A_EVENTREQ, dat);
	}


	unsigned long get_event(void)
	{
		return mem->read32(A_EVENT);
	}
	void set_event(unsigned long dat)
	{
		mem->write32(A_EVENT, dat);
	}
	void bset_event(unsigned long dat)
	{
		dat|=mem->read32(A_EVENT);
		mem->write32(A_EVENT, dat);
	}


	unsigned short get_timerstat(void)
	{
		return mem->read16_u(A_TIMERSTAT);
	}
	void set_timerstat(unsigned short stat)
	{
		mem->write16(A_TIMERSTAT, stat);
	}


	void set_keyport(unsigned short state)
	{
		mem->write16(A_KEY_PORT, state);
	}

	unsigned short get_keyport(void)
	{
		return mem->read16_u(A_KEY_PORT);
	}

	void clear_lcdSyncSignal(void)
	{
		mem->write8(0x00ffd4, 0);
	}

	void set_lcdSyncSignal(void)
	{
		mem->write8(0x00ffd4, 0x40);
	}

	void set_lcdPlace(int x, int y)
	{
		mem->write16(A_LCD_POS_X_PORT, x);
		mem->write16(A_LCD_POS_Y_PORT, y);
	}
	void get_lcdPlace(int *x, int *y)
	{
		*x=mem->read16_s(A_LCD_POS_X_PORT);
		*y=mem->read16_s(A_LCD_POS_Y_PORT);
	}
};
// extern RupAddress *systemAddress;

#endif
