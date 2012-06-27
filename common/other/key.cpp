/*
 * key.cpp - key api emulation routine
 *
 * $Id: key.cpp,v 1.15 2006/02/18 08:45:09 fumi Exp $
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

#include "cpu.h"
#include "keybuffer.h"
#include "rupaddress.h"
#include "state.h"

/* d1 return key */
int KeyBuffer::emu_bi_getbtn(Register *regs)
{
	unsigned short key;
	unsigned char c;

	c=systemAddress->get_eventregister();
	if(c)
	{
// クリアするのかは不明？
		systemAddress->set_eventregister(0);
		regs->d[1]=0x8000 | c;
		return 1;
	}

	key=deQueueKey();

	if(key == 0xffff)
		return 0;


	regs->d[1]=(key & ~(KEY_PUSH_UP | KEY_PUSH_DOWN));

	return 1;
}


int KeyBuffer::emu_Keyin(Register *regs)
{
	unsigned short key;

	if( ruputerState->isEnding() )
	{
		regs->d[ 0 ] = 0x8001; // event is on
		return 1;
	}

#if 1
	int c;
	c = systemAddress->get_eventregister();
	if(c)
	{
// クリアするのかは不明？
		systemAddress->set_eventregister(0);

// 本来はこの値を適当に設定し、
// EventCall あたりで考慮するのが普通かしら。
		regs->d[0] = 0x8000 | c;
		return 1;
	}
#endif


	for(;;)
	{
		key=deQueueKey();
//		fprintf(stderr, "key in dequeue out %x!!!!", key);
		if(key == 0xffff)
			return 0;

// KEY_PUSH_UP は捨てる

		if(key & KEY_PUSH_DOWN || key & 0x8000)
		{
#ifdef DEBUG
			fprintf(stderr, "key in out %x!!!!", key);
#endif

			regs->d[0]=key & ~KEY_PUSH_DOWN;
			return 1;
		}
	}
}

void KeyBuffer::emu_bi_sensbtn(Register *regs)
{
	unsigned short k;

	k=checkQueueKey();
	if(k != 0)
	{
		regs->d[1]=k & ~(KEY_PUSH_UP | KEY_PUSH_DOWN);;
		regs->d[2]=1;
	}
	else
		regs->d[2]=0;

}
