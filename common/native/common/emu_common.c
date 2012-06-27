/*
 * emu_common.c - common definition code (MN10200 code)
 *
 * $Id: emu_common.c,v 1.2 2006/01/21 18:04:35 fumi Exp $
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

#include "emu_common.h"

char buffer_for_gv_get[600];

static int g_font = 0;
static enum LANG_FLAG g_lang = 0;

void gv_kput2_init(int font, enum LANG_FLAG lang)
{
	g_font = font;
	g_lang = lang;
}

int gv_kput2( int x, int y, char const *str, int gap, int lop )
{
	switch( g_lang )
	{
	case USE_DEFAULT:
		return emu_gv_kput(x, y, str, g_font, gap, lop);
	case USE_JAPANESE:
		return emu_gv_kput2(x, y, str, g_font, gap, lop, 0);
	case USE_ISO8859_1:
		return emu_gv_kput2(x, y, str, g_font, gap, lop, 1);
	}

/* not reached */
	return 0;
}


