/*
 * rommmp.c - emulate rommmp data
 *
 * $Id: rommmp.cpp,v 1.3 2004/11/12 19:06:11 fumi Exp $
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

#include "mmpdata.h"
#include "memory.h"
#include "ruputerexception.h"

RomMmp::RomMmp(void)
{
	startaddress=A_ROMMMP_START;
	endaddress=startaddress+sizeof(romdata);
}

RomMmp::~RomMmp(void)
{
}

bool RomMmp::isRomMmpArea(unsigned long addr)
{
	if(addr >= startaddress && addr < endaddress)
		return true;
	return false;
}

unsigned long RomMmp::getOffset(unsigned long addr)
{
	return addr-startaddress;
}


bool RomMmp::read8(unsigned long addr, unsigned char *c)
{
	if(!isRomMmpArea(addr))
		return false;

	unsigned long offset;
	offset=getOffset(addr);

	*c=romdata[offset];

	return true;
}

bool RomMmp::read16(unsigned long addr, unsigned short *c)
{
	if(!isRomMmpArea(addr))
		return false;

	unsigned long offset;
	offset=getOffset(addr);

	MemoryForInside mem(romdata);
	*c=mem.read16_u(offset);

	return true;
}

bool RomMmp::read24(unsigned long addr, unsigned long *c)
{
	if(!isRomMmpArea(addr))
		return false;

	unsigned long offset;
	offset=getOffset(addr);

	MemoryForInside mem(romdata);
	*c=mem.read24(offset);

	return true;
}



unsigned char *RomMmp::getMemoryPointer(unsigned long addr)
{
	if( ! isRomMmpArea(addr) )
		throw MemoryBoundaryException();

	unsigned long offset;
	offset=getOffset(addr);

	return romdata+offset;
}
