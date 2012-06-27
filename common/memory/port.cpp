/*
 * port.cpp -
 *
 * $Id: port.cpp,v 1.14 2006/01/25 09:43:15 fumi Exp $
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


#include "cmlcd.h"
#include "port.h"
#include "lcdport.h"
#include "ruputer.h"
#include "address.h"
#include "keybuffer.h"

Port::Port(CMLcd *cmlcd, Ruputer *ruputer)
{
	this->cmlcd = cmlcd;
	this->ruputer = ruputer;

//	lcdport=new LcdPort(cmlcd);
	lcdport = new LcdPort(cmlcd);
}

Port::~Port(void)
{
	delete lcdport;
}

/*
 * Port に読み書きするメソッド
 *
 */
bool Port::read8(unsigned long addr, unsigned char *c)
{
#ifdef KEY_WITHOUT_PUSH_UP
	if(addr == A_KEY_PORT)
		ruputer->getKeyBuffer()->clearState();
#endif
	if(lcdport->readLcdPort8(addr, c))
		return true;

	return false;
}

bool Port::read16(unsigned long addr, unsigned short *c)
{
#ifdef KEY_WITHOUT_PUSH_UP
	if(addr == A_KEY_PORT || addr-1 == A_KEY_PORT)
		ruputer->getKeyBuffer()->clearState();
#endif
	if(lcdport->readLcdPort16(addr, c))
	   return true;
	return false;
}

bool Port::read24(unsigned long addr, unsigned long *c)
{
#ifdef KEY_WITHOUT_PUSH_UP
	if(addr <= A_KEY_PORT && addr-2 >= A_KEY_PORT)
		ruputer->getKeyBuffer()->clearState();
#endif
	if(lcdport->readLcdPort24(addr, c))
		return true;
	return false;
}


bool Port::write8(unsigned long addr, unsigned char c)
{
	if(lcdport->writeLcdPort8(addr, c))
		return true;

	return false;
}


bool Port::write16(unsigned long addr, unsigned short c)
{
	if(lcdport->writeLcdPort16(addr, c))
		return true;

	return false;
}
bool Port::write24(unsigned long addr, unsigned long c)
{
	if(lcdport->writeLcdPort24(addr, c))
		return true;

	return false;
}


