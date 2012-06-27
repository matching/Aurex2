/*
 * memorycpu.cpp - read/write function for cpu
 *
 * $Id: memorycpu.cpp,v 1.19 2004/11/10 04:02:36 fumi Exp $
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

#include "memorycpu.h"
#include "port.h"
#include "ruputer.h"
#include "internalfont.h"
#include "rommmp.h"
#include "option.h"
#include "ruputerexception.h"

// 本当はinlineにした方がいいのかしらん？

MemoryForCpu::MemoryForCpu(Port *port, InternalFont *ifont, RomMmp *rommmp)
{
	this->port = port;
	this->ifont = ifont;
	this->rommmp = rommmp;
}

MemoryForCpu::~MemoryForCpu(void)
{
	delete port;
	delete ifont;
	delete rommmp;
}

unsigned char MemoryForCpu::read8_0e(unsigned long addr)
{
	unsigned char c;

	if(port->read8(addr, &c))
		return c;
	if(ifont->read8(addr, &c))
		return c;
	if(rommmp->read8(addr, &c))
		return c;


	return readMemory8(addr);
}

unsigned long MemoryForCpu::read8_se(unsigned long addr)
{
	unsigned char c;

	if(port->read8(addr, &c))
		return signExtend8(c);
	if(ifont->read8(addr, &c))
		return signExtend8(c);
	if(rommmp->read8(addr, &c))
		return signExtend8(c);

	return signExtend8(readMemory8(addr));
}

unsigned short MemoryForCpu::read16_0e(unsigned long addr)
{
	warnOddAddress(addr);

	unsigned short c;

	if(port->read16(addr, &c))
		return c;
	if(ifont->read16(addr, &c))
		return c;
	if(rommmp->read16(addr, &c))
		return c;

	return readMemory16(addr);
}

unsigned long MemoryForCpu::read16_se(unsigned long addr)
{
	warnOddAddress(addr);

	unsigned short c;

	if(port->read16(addr, &c))
		return signExtend16(c);
	if(ifont->read16(addr, &c))
		return signExtend16(c);
	if(rommmp->read16(addr, &c))
		return signExtend16(c);


	return signExtend16(readMemory16(addr));
}

unsigned long MemoryForCpu::read24(unsigned long addr)
{
	warnOddAddress(addr);

	unsigned long c;
	if(port->read24(addr, &c))
		return c;
	if(ifont->read24(addr, &c))
		return c;
	if(rommmp->read24(addr, &c))
		return c;

	return readMemory24(addr);
}


//	unsigned long read16_se24(unsigned long addr);

void MemoryForCpu::write8(unsigned long addr, unsigned char data)
{
	if(port->write8(addr, data))
		return;

	writeMemory8(addr, data);

}

void MemoryForCpu::write8(unsigned long addr, unsigned long data)
{
	if(port->write8(addr, data))
		return;

	writeMemory8(addr, (unsigned char)data);
}

void MemoryForCpu::write16(unsigned long addr, unsigned short data)
{
	warnOddAddress(addr);

	if(port->write16(addr, data))
		return;

	writeMemory16(addr, data);

}

void MemoryForCpu::write16(unsigned long addr, unsigned long data)
{
	warnOddAddress(addr);

	if(port->write16(addr, data))
		return;

	writeMemory16(addr, (unsigned short)data);

}

void MemoryForCpu::write24(unsigned long addr, unsigned long data)
{
	warnOddAddress(addr);

	if(port->write24(addr, data))
		return;

	writeMemory24(addr, data);
}

/**
 * addr が奇数なら、奇数アドレスの多バイト転送命令の警告を出す。
 * 警告はどう出すかは微妙。
 * → Exception を投げる！
 *
 */
void MemoryForCpu::warnOddAddress(unsigned long addr)
{
	if(addr & 1)
	{
#ifdef DEBUG
		fprintf(stderr, "ERROR !! Multibyte access to odd address of memory.\n");
#endif
		if( option->get_warn_odd_access() )
			throw OddAccessException(addr);
	}
}
