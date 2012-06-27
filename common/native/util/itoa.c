/*
 * itoa.c -
 *
 * $Id: itoa.c,v 1.1 2004/04/29 09:44:33 fumi Exp $
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

char *util_uitoa_supply_char(unsigned int val, char *str, int cols, int pad)
{
	int c;

	str[cols]=0;
	while(cols-- && val > 0)
	{
		c=val % 10;
		val/=10;

		str[cols]='0' + c;
	}

	while(cols >= 0)
	{
		str[cols]=pad;
		cols--;
	}

	return str;
}

char *util_uitoa(unsigned int val, char *str)
{
	int c;
	char *p, tmp;

	if(val == 0)
	{
		str[0]='0';
		str[1]='\0';
		return str;
	}

	p=str;
	for(; val > 0; p++)
	{
		c=val%10;
		val/=10;

		*p=c+'0';
	}

	*p=0;
	p--;

/* str : 頭、p: さいご、で文字列をひっくり返す */
	for(; str < p; str++, p--)
	{
/* swap */
		tmp=*p;
		*p=*str;
		*str=tmp;
	}
	return str;
}
