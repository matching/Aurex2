/*
 * sdfindfile.cpp - findfile emulation wrapper class
 *
 * $Id: sdfindfile.cpp,v 1.2 2006/02/22 17:40:39 fumi Exp $
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

#include "sdfindfile.h"

SDFindFile::SDFindFile(struct dta_t *dta_area, char const *pathname, unsigned char search_attribute)
{
	assert( sizeof(struct dta_t) == 44 );

	this->dta_area=dta_area;
	this->pathname=pathname;
	this->search_attribute=search_attribute;
}

SDFindFile::~SDFindFile()
{
// do nothing
}

/**
 * @param year 西暦を指定（1980からのオフセットにはしない）
 * @param month 1-12
 * @param day 1-31
 */
void SDFindFile::setDate(int year, int month, int day)
{
	year -= 1980;
#if 0
	month &= 0x0f;
	day &= 0x1f;
#endif

	dta_area->date[1] = (year << 1) | (month >> 3);
	dta_area->date[0] = (month << 5) | day;
}

/**
 * @param hour 0-23
 * @param min 0-59
 * @param sec 0-60
 */
void SDFindFile::setTime(int hour, int min, int sec)
{
#if 0
	hour &= 0x1f;
	min &= 0x3f;
#endif
	sec >>= 1;
	dta_area->time[1] = (hour << 3) | (min >> 3);
	dta_area->time[0] = (min  << 5) | sec;
}
