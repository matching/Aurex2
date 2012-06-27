/*
 * strcpy11.c - strcpy*** emulation code (for Mn10200 code)
 *
 * $Id: strcpy1.c,v 1.2 2006/02/10 17:25:26 fumi Exp $
 *
 * Copyright (C) 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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


#include <string.h>

void sub_strcpy1(char *str1, char const *str2)
{
	(void)strcpy(str1, str2);
}


void sub_strcpy2(char *str1, char const *str2, char const *str3)
{
	(void)strcpy(str1, str2);
	(void)strcat(str1, str3);
}

void sub_strcpy3(char *str1, char const *str2, char const *str3, char const *str4)
{
	(void)strcpy(str1, str2);
	(void)strcat(str1, str3);
	(void)strcat(str1, str4);
}


void sub_strcpyadd0(char *str1, char const *str2, int n)
{
	(void)strncpy(str1, str2, n);
	str1[n] = 0;
}


void sub_StrCpy(char *str1, char const *str2)
{
	char c;

	while( *str2 )
	{
		if( *str2 >= 'a' && *str2 <= 'z' )
			c = *str2 - 'a' + 'A';
		else
			c = *str2;

		*str1++ = c;
		str2++;
	}
}


void sub_wstrcpy(char *str1, char const *str2, char *str3, char const *str4)
{
	(void)strcpy(str1, str2);
	(void)strcpy(str3, str4);
}

