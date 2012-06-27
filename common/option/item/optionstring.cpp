/*
 * optionstring.cpp - Handler to string option
 *
 * $Id: optionstring.cpp,v 1.5 2004/11/12 19:06:11 fumi Exp $
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
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "optionitem.h"

OptionString::OptionString(char const *key, char const *init) : OptionItem(key)
{
	value=NULL;

	setStringValue(init);
}

OptionString::~OptionString(void)
{
	delete[] value;
}


void OptionString::setStringValue(char const *v)
{
#ifdef DEBUG
	fprintf(stderr, "set string value %p vs %p\n", value, v);
	fflush(stderr);
#endif

	if(v == NULL)
	{
		delete[] value;
		value=NULL;
		return;
	}

	if(value && strcmp(value, v) == 0)
		return;

	delete[] value;


	int len;

	len=strlen(v);
	value=new char[len+1];
	memcpy(value, v, len+1);
}

char const *OptionString::getStringValue(void)
{
	return (char const *)value;
}

void OptionString::getValueString(char *out)
{
	assert( out );

	if(value)
		strcpy(out, value);
	else
		*out=0;
}

bool OptionString::setValueString(char const *value)
{
	setStringValue(value);
	return true;
}
