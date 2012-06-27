/*
 * keybuffer.cpp - having key buffering and key state
 *
 * $Id: keybuffer.cpp,v 1.19 2006/03/06 12:58:17 fumi Exp $
 *
 * Copyright (C) 2004, 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

#include "config.h"
#include "keybuffer.h"
#include "rupaddress.h"

#include "log.h"

KeyBuffer::KeyBuffer(RupAddress *systemAddress, RuputerState *ruputerState)
{
	this->systemAddress = systemAddress;
	this->ruputerState = ruputerState;

	assert( systemAddress );
	assert( ruputerState );

	head=0;
	tail=0;
	state=0;
}


unsigned short KeyBuffer::enQueueKey(unsigned short code)
{
	int test;

#ifdef DEBUG
	fprintf(stderr, "!! enqueue %x\n", code);
	fflush(stderr);
#endif

	updateState(code);

	test=tail+1;
	if(test >= KEY_BUF_SIZE)
		test%=KEY_BUF_SIZE;

	if(test == head)
	{
		Log::getInstance()->output(Log::Warning, "\nWarning. Key buffer is full. key %02x Cannot be enqueued.\n", code );
		return 0xffff;
	}

	buf[tail++]=code;
	if(tail == KEY_BUF_SIZE)
		tail=0;

	return 0;
}

unsigned short KeyBuffer::deQueueKey(void)
{
	int key;

	if(head == tail)
		return 0xffff;

	key=buf[head++];
	if(head == KEY_BUF_SIZE)
		head=0;

#ifdef DEBUG
	fprintf(stderr, "!! dequeue %x\n", key);
	fflush(stderr);
#endif

	return key;
}

unsigned short KeyBuffer::checkQueueKey(void)
{
	if(head == tail)
		return 0xffff;

	return buf[head];
}

void KeyBuffer::updateState(unsigned short code)
{
#ifdef KEY_WITHOUT_PUSH_UP
	state|=code & ~KEY_PUSH_DOWN;
	systemAddress->set_keyport(state);
#else
	if(code & KEY_PUSH_DOWN)
		state|=(code & ~KEY_PUSH_DOWN);
	else if(code & KEY_PUSH_UP)
		state&=~(code & ~KEY_PUSH_DOWN);

	systemAddress->set_keyport(state);
#endif
}

void KeyBuffer::clearState(void)
{
	state=0;
}

unsigned short KeyBuffer::getNowState(void)
{
	return state;
}



void KeyBuffer::bsetFirstItem(unsigned short bit)
{
#if 0
	if(head == tail)
		buf[head]|=bit;
	else
		enQueueKey(bit);
#endif
	enQueueKey(bit);
}


void KeyBuffer::flush(void)
{
	head=0;
	tail=0;
	state=0;
}
