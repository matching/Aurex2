/*
 * romgaiji.cpp - Handle Gaiji region
 *
 * $Id: romgaiji.cpp,v 1.2 2004/11/03 14:02:18 fumi Exp $
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

#include "romgaiji.h"
#include "gaijidata.h"

RomGaiji::RomGaiji(void)
{
	ngaiji=sizeof(gaijidata)/sizeof(struct GaijiData);
}

RomGaiji::~RomGaiji(void)
{
}

bool RomGaiji::isGaiji(unsigned short code)
{
	int start=0;
	int mid;
	int end=ngaiji;

	for(;;)
	{
		mid=(start+end)/2;
		if(start >= end)
			break;


		if(gaijidata[mid].code == code)
			return true;
		else if(gaijidata[mid].code < code)
		{
			start=mid+1;
		}
		else // gaijidata[mid].code > code
		{
			end=mid;
		}
	}

	return false;
}

unsigned char *RomGaiji::getGaijiData(unsigned short code)
{
	int start=0;
	int mid;
	int end=ngaiji;

	for(;;)
	{
		mid=(start+end)/2;
		if(start >= end)
			break;

		if(gaijidata[mid].code == code)
			return gaijidata[mid].data;
		else if(gaijidata[mid].code < code)
		{
			start=mid+1;
		}
		else // gaijidata[mid].code > code
		{
			end=mid;
		}
	}

	return NULL;
}

