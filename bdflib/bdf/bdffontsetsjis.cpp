/*
 * bdffontsetsjis.cpp - JIS fontset class
 *
 * $Id: bdffontsetsjis.cpp,v 1.5 2004/11/05 04:12:10 fumi Exp $
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
#include "bdffontset.h"
#include "bdffontsetsjis.h"

BdfFontSetsJis::BdfFontSetsJis(BdfFontSet *jisx0201, BdfFontSet *jisx0208)
{
	this->jisx0201=jisx0201;
	this->jisx0208=jisx0208;
}

BdfFontSetsJis::~BdfFontSetsJis(void)
{
}

/** フォントデータを得る
 * code はEUCとして貰える
 *
 *
 */

BdfFont *BdfFontSetsJis::getFontData(unsigned short code)
{
	BdfFontSet *target = NULL;

	if(code > 0xff)
	{
		if((code >> 8) == 0x8e)
		{
	// 半角カナ
			code&=0xff;
			target=jisx0201;
		}
		else
		{
	// 普通の漢字
	// EUCなので両方の 0x80 落とすとJIS
			code&=0x7f7f;
			target=jisx0208;
		}
	}
	else
	{
		target=jisx0201;
	}

	if(target == NULL)
		return NULL;

	return target->getBdfFont(code);
}
