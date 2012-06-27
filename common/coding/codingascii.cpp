/*
 * codingascii.cpp - Coding system handle class for ASCII
 *
 * $Id: codingascii.cpp,v 1.3 2005/01/13 08:40:42 fumi Exp $
 *
 * Copyright (C) 2005 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

#include "codingascii.h"



CodingAscii::CodingAscii(char const *str)
{
	len = strlen( str );
	buf = new unsigned char[len + 1];

	memcpy(buf, str, len + 1);
}

CodingAscii::~CodingAscii(void)
{
	delete[] buf;
}


int CodingAscii::searchForward(int start, char c)
{
	char *p;

	p = strchr( (char *)buf + start, c );
	if( p == NULL )
		return -1;

	return p - (char *)buf;
}

int CodingAscii::searchReverse(char c)
{
	char *p;

	p = strrchr( (char *)buf, c );
	if( p == NULL )
		return -1;

	return p - (char *)buf;
}


void CodingAscii::subString(int start, int end, char *out)
{

	if( end == -1 )
	{
		strcpy(out, (char *)buf + start);
		return;
	}

	int diff;

	diff = end - start;

	strncpy( out, (char *)buf + start, diff );
	out[diff] = 0;
}

void CodingAscii::replaceCharacter(char from, char to)
{
	unsigned char *p;

	p = buf;

	while( *p )
	{
		if( *p == from )
			*p = to;
		p++;
	}
}

void CodingAscii::toString(char *out)
{
	strcpy( out, (char *)buf );
}


#ifdef UNIT_TEST

/*
 *
 * g++ -I../include -DUNIT_TEST codingbase.cpp codingascii.cpp
 */

int main(void)
{
	CodingAscii asc("foo/bar");

	char buf1[1024];
	char buf2[1024];
	int n;

	asc.replaceCharacter('/', '\\');
//	asc.toString(buf);
//	asc.subString(1, 4, buf);


	n = asc.searchForward(0, '\\');

	asc.subString(0, n, buf1);
	asc.subString(n + 1, -1, buf2);

	printf("str: %d, (%s, %s)\n", n, buf1, buf2);

	return 0;
}

#endif
