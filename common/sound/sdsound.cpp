/*
 * sdsound.cpp - System depending sound class (base classs)
 *
 * $Id: sdsound.cpp,v 1.1 2006/02/08 15:26:42 fumi Exp $
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


#include "sdsound.h"

SDSound::SDSound(void)
{
	state = INITIAL;
}

SDSound::~SDSound(void)
{
//	close();
}


/**
 *
 * @retval int -1 ¼ºÇÔ
 * @retval int 0 À®¸ù
 */
int SDSound::output(unsigned char const *buf, int len)
{
	open();

	if( state != OPEN )
		return -1;

	return subOutput(buf, len);
}


int SDSound::open(void)
{
	if( state == OPEN )
		return 0;

	int i = subOpen();
	if( i != 0 )
		return -1;

	state = OPEN;
	return 0;
}

#if 0
int SDSound::close(void)
{
	if( state == INITIAL )
		return 0;

	int i = subClose();
	if( i != 0 )
		return -1;

	state = INITIAL;
	return 0;
}

#endif
