/*
 * dispyn.c - dispyn(b) emulation code (for Mn10200 code)
 *
 * $Id: dispyn.c,v 1.5 2006/01/21 18:04:35 fumi Exp $
 *
 * Copyright (C) 2004, 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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


#include "emu_lcdbios.h"
#include "emu_rupsys.h"
#include "emu_font.h"

#include "messages_sjis.h"

static void reverse_yn_button(int yn, int y);
static int input_yn(int yn, int y);

int sub_dispYN(char *mes1, char *mes2, int init)
{
	emu_gv_put(1, 6, (char *)mmpYN, 0);


	emu_gv_kput(4, 8   , mes1, 0, 0, 0);
	emu_gv_kput(4, 8+13, mes2, 0, 0, 0);

	return input_yn(init, 39);
}


int sub_dispYNb(char *mes1, char *mes2, int init)
{
	emu_gv_put(0, 0, (char *)mmpYNb, 0);


	emu_gv_kput(2, 1   , mes1, 0, 0, 0);
	emu_gv_kput(2, 1+13, mes2, 0, 0, 0);

	return input_yn(init, 41);
}



int sub_cancelCHK(void)
{
	emu_gv_put(0, 0, (char *)mmpYNb, 0);

	emu_gv_kput(8, 10, CANCELCHK_MSG1, 0, 0, 0);
	emu_gv_kput(8, 24, CANCELCHK_MSG2, 0, 0, 0);

	return input_yn(0, 41);
}

static int input_yn(int yn, int y)
{
	int key;
	int rst;

//	yn=init; /* yes:0 no:1 */
	reverse_yn_button(yn, y);
	for(;;)
	{
		key=emu_Keyin(0xff, -1);
		rst=emu_EventCall(key);
		if(rst == -1)
			return -2; // ½ªÎ»Í×µá

		if(key & Bkey_B)
			return yn;

		if(key & Bkey_rg && yn == 0)
		{
			reverse_yn_button(yn, y);
			yn=1;
			reverse_yn_button(yn, y);
		}

		if(key & Bkey_lf && yn == 1)
		{
			reverse_yn_button(yn, y);
			yn=0;
			reverse_yn_button(yn, y);
		}
	}

	return 0;
}

#define YESNO_WIDTH 25
#define YESNO_HEIGHT 12

#define YES_X 11
#define NO_X 57

static void reverse_yn_button(int yn, int y)
{
	if(yn == 0)
		emu_gv_reverse(YES_X, y, YES_X+YESNO_WIDTH, y+YESNO_HEIGHT);
	else
		emu_gv_reverse(NO_X, y, NO_X+YESNO_WIDTH, y+YESNO_HEIGHT);
}




/******* US *******/

int sub_dispYN_us(char *mes1, char *mes2, int init)
{
	emu_gv_put(1, 6, (char *)mmpYN, 0);


	emu_gv_kput2(4, 8   , mes1, 5, 0, 0, 1);
	emu_gv_kput2(4, 8+13, mes2, 5, 0, 0, 1);

	return input_yn(init, 39);
}


int sub_dispYNb_us(char *mes1, char *mes2, int init)
{
	emu_gv_put(0, 0, (char *)mmpYNb, 0);


	emu_gv_kput2(2, 1   , mes1, 5, 0, 0, 1);
	emu_gv_kput2(2, 1+13, mes2, 5, 0, 0, 1);

	return input_yn(init, 41);
}

