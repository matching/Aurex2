/*
 * memoryinside.cpp - read/write functions for internal function
 *
 * $Id: memoryinside.cpp,v 1.5 2004/02/20 08:28:06 fumi Exp $
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
#include "memory.h"

MemoryForInside::MemoryForInside(void) : MemoryBase(Memory::main_memory)
{
	baseaddr=0UL;
}

MemoryForInside::MemoryForInside(unsigned long addr) : MemoryBase(Memory::main_memory)
{
	baseaddr=addr;
}

MemoryForInside::MemoryForInside(unsigned char *buf) : MemoryBase(buf)
{
	baseaddr=0;
}

unsigned char MemoryForInside::read8_u(unsigned long offset)
{
	return readMemory8(baseaddr+offset);
}

signed char MemoryForInside::read8_s(unsigned long offset)
{
	return (signed char)readMemory8(baseaddr+offset);
}

unsigned short MemoryForInside::read16_u(unsigned long offset)
{
	return readMemory16(baseaddr+offset);
}
signed short MemoryForInside::read16_s(unsigned long offset)
{
	return (signed short)readMemory16(baseaddr+offset);
}

unsigned long MemoryForInside::read24(unsigned long offset)
{
	return readMemory24(baseaddr+offset);
}

unsigned long MemoryForInside::read32(unsigned long offset)
{
	return readMemory32(baseaddr+offset);
}


void MemoryForInside::write8(unsigned long offset, unsigned char data)
{
	writeMemory8(baseaddr+offset, data);
}
void MemoryForInside::write16(unsigned long offset, unsigned short data)
{
	writeMemory16(baseaddr+offset, data);
}
void MemoryForInside::write24(unsigned long offset, unsigned long data)
{
	writeMemory24(baseaddr+offset, data);
}
void MemoryForInside::write32(unsigned long offset, unsigned long data)
{
	writeMemory32(baseaddr+offset, data);
}


#if 0
void *MemoryForInside::getMemoryPointer(unsigned long offset)
{
	return (void *)(memory+baseaddr+offset);
}
#endif
