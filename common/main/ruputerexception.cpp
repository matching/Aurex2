/*
 * ruputerexception.cpp - Ruputer's exception
 *
 * $Id: ruputerexception.cpp,v 1.4 2006/01/09 07:43:15 fumi Exp $
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

#include "ruputerexception.h"
#include "cpu.h"

char RuputerException::regsString[ 256 ];

RuputerException::RuputerException(void)
{
	this->regs = NULL;
}

RuputerException::RuputerException(Register *regs)
{
	this->regs = regs;
}

RuputerException::~RuputerException(void)
{
}

void RuputerException::getRegisterString(char *out)
{
	if( regs == NULL )
		strcpy(out, "unknown register value");
	else
		regs->toString(out);
}

char const *RuputerException::getRegisterString(void)
{
	if( regs == NULL )
		strcpy(regsString, "unknown register value");
	else
		regs->toString(regsString);

	return regsString;
}


OddAccessException::OddAccessException(unsigned long addr) // : RuputerException()
{
	this->addr = addr;
}

OddAccessException::OddAccessException(Register *regs, unsigned long addr) : RuputerException(regs)
{
	this->addr = addr;
}

unsigned long OddAccessException::getEffectiveAddress(void)
{
	return this->addr;
}


UndefinedInstructionException::UndefinedInstructionException(Register *regs) : RuputerException(regs)
{
}


MemoryBoundaryException::MemoryBoundaryException(void)
{
}
