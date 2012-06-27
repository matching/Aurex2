/*
 * memorybase.cpp - System depend  memory read/write functions
 *
 * $Id: memorybase.cpp,v 1.10 2006/03/06 12:58:17 fumi Exp $
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

#include "config.h"
#include "memory.h"

MemoryBase::MemoryBase(unsigned char *_memory)
{
	memory=_memory;
}

int MemoryBase::isAddressOver(unsigned long addr)
{
	if(addr < 0 || addr > MEMORY_SIZE)
		return 1;
	return 0;
}

unsigned char MemoryBase::readMemory8(unsigned long addr)
{
	if(isAddressOver(addr))
		return 0;

#ifdef MEMORY_DEBUG
	fprintf(stderr, "mem read8 effective address %06x\n", addr);
#endif


	return memory[addr];
}
void MemoryBase::writeMemory8(unsigned long addr, unsigned char data)
{
	if(isAddressOver(addr))
		return;

#ifdef MEMORY_DEBUG
	fprintf(stderr, "mem write8 effective address %06x\n", addr);
#endif

	memory[addr]=data;
}

#ifdef FAIL_NOT_ALIGNED_ADDRESS
unsigned short MemoryBase::readMemory16(unsigned long addr)
{
	if(isAddressOver(addr))
		return 0;
	return memory[addr] | (memory[addr+1] << 8);
}

unsigned long MemoryBase::readMemory24(unsigned long addr)
{
	if(isAddressOver(addr))
		return 0;

	return memory[addr] | (memory[addr+1] << 8) | (memory[addr+2] << 16);
}

unsigned long MemoryBase::readMemory32(unsigned long addr)
{
	if(isAddressOver(addr))
		return 0;
	return memory[addr] | (memory[addr+1] << 8) | (memory[addr+2] << 16) | (memory[addr+3] << 24);
}


void MemoryBase::writeMemory16(unsigned long addr, unsigned short data)
{
	if(isAddressOver(addr))
		return;

	memory[addr]=data;
	memory[addr+1]=data >> 8;
}

void MemoryBase::writeMemory24(unsigned long addr, unsigned long data)
{
	if(isAddressOver(addr))
		return;

	memory[addr]=data;
	memory[addr+1]=data >> 8;
	memory[addr+2]=data >> 16;
}

void MemoryBase::writeMemory32(unsigned long addr, unsigned long data)
{
	if(isAddressOver(addr))
		return;

	memory[addr]=data;
	memory[addr+1]=data >> 8;
	memory[addr+2]=data >> 16;
	memory[addr+3]=data >> 24;
}

#else // ifdef FAIL_NOT_ALIGNED_ADDRESS

# ifndef HOST_IS_BIG_ENDIAN

unsigned short MemoryBase::readMemory16(unsigned long addr)
{
	if(isAddressOver(addr))
		return 0;

#ifdef MEMORY_DEBUG
	fprintf(stderr, "mem read16 effective address %06x\n", addr);
#endif


	return *(unsigned short *)(memory+addr);
}

unsigned long MemoryBase::readMemory24(unsigned long addr)
{
	if(isAddressOver(addr))
		return 0;

#ifdef MEMORY_DEBUG
	fprintf(stderr, "mem read24 effective address %06x\n", addr);
#endif


	return (*(unsigned long *)(memory+addr) & 0xffffffUL);
}

unsigned long MemoryBase::readMemory32(unsigned long addr)
{
	if(isAddressOver(addr))
		return 0;

#ifdef MEMORY_DEBUG
	fprintf(stderr, "mem read32 effective address %06x\n", addr);
#endif


	return *(unsigned long *)(memory+addr);
}


void MemoryBase::writeMemory16(unsigned long addr, unsigned short data)
{
	if(isAddressOver(addr))
		return;

#ifdef MEMORY_DEBUG
	fprintf(stderr, "mem write16 effective address %06x\n", addr);
#endif


	*(unsigned short *)(memory+addr)=data;
}

void MemoryBase::writeMemory24(unsigned long addr, unsigned long data)
{
	if(isAddressOver(addr))
		return;

#ifdef MEMORY_DEBUG
	fprintf(stderr, "mem write24 effective address %06x\n", addr);
#endif

	*(unsigned short *)(memory+addr)=data & 0xffff;
	*(unsigned char *)(memory+addr+2)=data >> 16;
}

void MemoryBase::writeMemory32(unsigned long addr, unsigned long data)
{
	if(isAddressOver(addr))
		return;
	*(unsigned long *)(memory+addr)=data;
}

# else // ifndef HOST_IS_BIG_ENDIAN

static unsigned short swap16(unsigned short d)
{
	return ((d >> 8) & 0xff) | ((d & 0xff) << 8);
}
static unsigned long swap32(unsigned long d)
{
	return ((d << 24) | ((d << 8) & 0x00FF0000) | ((d >> 8) & 0x0000FF00) | (d >> 24));
}

unsigned short MemoryBase::readMemory16(unsigned long addr)
{
	if(isAddressOver(addr))
		return 0;
	return swap16(*(unsigned short *)(memory+addr));
}

unsigned long MemoryBase::readMemory24(unsigned long addr)
{
	if(isAddressOver(addr))
		return 0;

	return swap32(*(unsigned long *)(memory+addr)) & 0xffffffUL;
}

unsigned long MemoryBase::readMemory32(unsigned long addr)
{
	if(isAddressOver(addr))
		return 0;

	return swap32(*(unsigned long *)(memory+addr));
}


void MemoryBase::writeMemory16(unsigned long addr, unsigned short data)
{
	if(isAddressOver(addr))
		return;
	*(unsigned short *)(memory+addr)=swap16(data);

}

void MemoryBase::writeMemory24(unsigned long addr, unsigned long data)
{
	if(isAddressOver(addr))
		return;

	*(unsigned short *)(memory+addr)=swap16(data & 0xffff);
	*(unsigned char *)(memory+addr+2)=data >> 16;
}

void MemoryBase::writeMemory32(unsigned long addr, unsigned long data)
{
	if(isAddressOver(addr))
		return;
	*(unsigned long *)(memory+addr)=swap32(data);
}


# endif // ifndef HOST_IS_BIG_ENDIAN
# endif // ifdef FAIL_NOT_ALIGNED_ADDRESS
