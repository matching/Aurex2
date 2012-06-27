/*
 * smallen.c - softkey small english table (for MN10200 code)
 *
 * $Id: smallen.c,v 1.2 2004/03/07 05:29:35 fumi Exp $
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

#include "emu_rupsys.h"
#include "emu_lcdbios.h"
#include "softkey.h"

static char eiji_table[][16]={
	"1234567890-^",
	"qwertyuiop@ ",
	"asdfghjkl;: ",
	"zxcvbnm,./  "};


struct softkey_mmp_info en_info[4]={
	{1, 6, 12},
	{4, 6, 11},
	{7, 6, 11},
	{10, 6, 10},};

static void smallen_reverse_button(int x, int y);

unsigned short smallen_process(int key, int *mode)
{
	static int x=11, y=0;

	if(key & Bkey_B)
		return eiji_table[y][x];

	if(key != 0)
		smallen_reverse_button(x, y);

	if(key & Bkey_rg)
	{
		if(x+1 >= en_info[y].col)
		{
			*mode=0;
			return 0;
		}
		x++;
	}

	if(key & Bkey_lf)
	{
		if(x-1 >= 0)
			x--;
	}

	if(key & Bkey_up)
	{
#if 0
		if(y == 0)
		{
			*mode=2;
			return 0;
		}
#endif
		if(y > 0)
		{
			y--;
			if(x >= en_info[y].col)
				x=en_info[y].col-1;
		}
	}

	if(key & Bkey_dw)
	{
		if(y <= 2)
		{
			y++;
			if(x >= en_info[y].col)
				x=en_info[y].col-1;
		}
	}

	smallen_reverse_button(x, y);

	return 0;
}



static void smallen_reverse_button(int x, int y)
{
	int px, py;

	px=en_info[y].start_x+x*(en_info[y].width+1);
	py=KEYTBL_Y_OFFSET+y*9;

	emu_gv_reverse(px, py, px+en_info[y].width-1, py+7);
}
