/*
 * exmemory.cpp - Load from external memory to internal memory (for native function)
 *
 * $Id: exmemory.cpp,v 1.4 2005/01/13 15:13:47 fumi Exp $
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
#include <string.h>

#include "exmemory.h"
#include "memory.h"
#include "address.h"

#include "native.h"

ExMemory::ExMemory(void)
{
	native();
}

void ExMemory::native(void)
{
	unsigned char *p;

	p=(unsigned char *)Memory::getMemoryPointer(A_RUPSYS_FUNCTION);
	memcpy(p, nativecode, sizeof(nativecode));
}
