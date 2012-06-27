/*
 * cessjis.cpp - Handling Japanese SJIS
 *
 * $Id: cessjis.cpp,v 1.7 2005/07/27 18:53:28 fumi Exp $
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

#include "cessjis.h"

CesSjis::CesSjis(void)
{
}
CesSjis::~CesSjis(void)
{
}

bool CesSjis::isShiftJis1(unsigned char c)
{
	return  (c >= 0x81 && c <= 0x9f) || (c >= 0xe0 && c <= 0xfc);
}

bool CesSjis::isShiftJis2(unsigned char c)
{
	return c >= 0x40 && c <= 0xfc && c != 0x7f;
}


/** SJISかどうかをチェックし、一文字のバイト数を返却
 * @param str 入力文字列
 * @retval 1 ASCII/半角カナ
 * @retval 2 漢字
 */
int CesSjis::checkChar(char const *str)
{
	if(isShiftJis1((unsigned char)*str))
		if(isShiftJis2((unsigned char)*(str+1)))
			return 2;
	return 1;
}

/** SJISかどうかをチェックし、一文字のバイト数を返却
 * @param str 入力文字列
 * @param achar 文字 一文字
 * @retval 1 ASCII/半角カナ
 * @retval 2 漢字
 */
int CesSjis::checkChar(char const *str, unsigned short *achar)
{
	if(isShiftJis1((unsigned char)*str))
		if(isShiftJis2((unsigned char)*(str+1)))
		{
			*achar = (((unsigned short)*(unsigned char *)str) << 8)
				| ((unsigned short)*(unsigned char *)(str + 1));
			return 2;
		}

	*achar = *(unsigned char*)str;
	return 1;
}


/** SJIS → EUC 変換を行なう
 * @param src SJIS（入力）
 * @param dat euc （出力）
 * @retval 1 失敗
 * @retval 0 成功
 */
int CesSjis::convToEuc(char const *src, char *dst)
{
	int len;
	unsigned char *out;
	unsigned char *str;

	str=(unsigned char *)src;
	out=(unsigned char *)dst;

	while(*str)
	{
		len=checkChar((char *)str);
		if(len == 1)
		{
			*out=*str;
			if(*out >= 0xa1 && *out <= 0xdf) // 1バイトカナ
			{
				*(out+1)=*out;
				*out=0x8e;
				out+=2;
			}
			else
				out+=1;
			str++;
		}
		else
		{
			if(*str <= 0x9F)
			{
				if(*(str+1) < 0x9F)
					*out=((int)*str << 1) - 0xE1;
				else
					*out=((int)*str << 1) - 0xE0;
			}
			else
			{
				if(*(str+1) < 0x9F)
					*out=((int)*str << 1) - 0x161;
				else
					*out=((int)*str << 1) - 0x160;
			}

			*out|=0x80;

			str++;
			out++;
			if(*str == 0)
			{
				*out=0;
				return 1;
			}


			*out=*str;

			if(*out < 0x7F)
				*out-=0x1F;
			else if(*out < 0x9F)
				*out-=0x20;
			else
				*out-=0x7E;

			*out|=0x80;
			str++;
			out++;
		}
	}

	*out=0;
	return 0;
}
