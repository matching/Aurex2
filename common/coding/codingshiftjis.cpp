/* -*-mode:c++; coding:shift_jis-unix-*-
 *
 * codingshiftjis.cpp - Coding system handle class for shift jis
 *
 * $Id: codingshiftjis.cpp,v 1.2 2005/01/06 14:36:13 fumi Exp $
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

#include "codingshiftjis.h"
#include "cessjis.h"

CodingShiftJis::CodingShiftJis(char const *str) : CodingAscii( str )
{
	cessjis = new CesSjis();
}

CodingShiftJis::~CodingShiftJis(void)
{
	delete cessjis;
}


int CodingShiftJis::searchForward(int start, char c)
{
	unsigned char *p;
	int n;

	p = buf + start;

	for(;;)
	{
		n = cessjis->checkChar( (char *)p );
		if( n == 1 )
		{
			if( *p == c )
				return p - buf;
		}

		p += n;

		if( *p == 0 )
			break;
	}

	return -1;
}

int CodingShiftJis::searchReverse(char c)
{
	int start = 0;
	int last = -1;
	int n;

	for(;;)
	{
		n = searchForward( start, c );
		if( n == -1 )
			break;

		last = n;
		start = n + 1;
	}
	return last;
}

void CodingShiftJis::replaceCharacter(char from, char to)
{
	unsigned char *p;
	int n;

	p = buf;

	for(;;)
	{
		n = cessjis->checkChar( (char *)p );
		if( n == 1 )
		{
			if( *p == from )
				*p = to;
		}

		p += n;

		if( *p == 0 )
			break;
	}
}

#ifdef UNIT_TEST_SHIFT_JIS

/*
 *
 * g++ -I../include -DUNIT_TEST_SHIFT_JIS codingbase.cpp codingshiftjis.cpp codingascii.cpp ../font/cessjis.cpp
 */

int main(void)
{
	CodingShiftJis asc("foo\\ar表\示fo\\パソ\コン\\ふー\\バー\\パソ\コン");

	char buf1[1024] = "";
	char buf2[1024] = "";
	int n;

//	asc.replaceCharacter('\\', '/');
//	asc.subString(1, 4, buf);

//	n = asc.searchForward(0, '\\');
	n = asc.searchReverse('\\');

	asc.subString(0, n, buf1);
	asc.subString(n + 1, -1, buf2);
	printf("str: %d, (%s, %s)\n", n, buf1, buf2);

	return 0;
}

#endif
