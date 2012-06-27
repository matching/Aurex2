/*
 * disprtc.c - dispRtc... emulation code (for Mn10200 code)
 *
 * $Id: disprtc.c,v 1.5 2005/01/06 14:21:33 fumi Exp $
 *
 * Copyright (C) 2004, 2005 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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
#include <string.h>

#include "emu_lcdbios.h"
#include "emu_rupsys.h"
#include "emu_font.h"

#include "messages_sjis.h"

#include "itoa.h"

static int *year  = (int *)(0x390+0x00);
static int *month = (int *)(0x390+0x02);
static int *day   = (int *)(0x390+0x04);
static int *week  = (int *)(0x390+0x06);
static int *hour  = (int *)(0x390+0x08);
static int *min   = (int *)(0x390+0x0a);
static int *sec   = (int *)(0x390+0x0c);

// dispNowWeek
void sub_dispRtcWeek(int x, int y, int font, int dot, int mode)
{
	static char *week_string[3][7]={
		DISPRTCWEEK_WEEK0,
		DISPRTCWEEK_WEEK1,
		DISPRTCWEEK_WEEK2
	};


	if(mode >= 0 && mode <= 2)
		emu_gv_kput(x, y, week_string[mode][*week], font, dot, 0);
}

//dispNowDate
void sub_dispRtcDate(int x, int y, int font, int dot, int YearNotDisp)
{
	char str[32] = "";

	if(YearNotDisp == 0)
	{
		util_uitoa(*year, str);
		strcat(str, "-");
	}

	util_uitoa_supply_char(*month, str+strlen(str), 2, ' ');
	strcat(str, "-");
	util_uitoa_supply_char(*day, str+strlen(str), 2, ' ');

	emu_gv_kput(x, y, str, font, dot, 0);
}


// dispNowAmPm
void sub_dispRtcAmPm(int x, int y, int font, int dot)
{
	char *str;

	if(*hour >= 12)
		str="PM";
	else
		str="AM";

	emu_gv_kput(x, y, str, font, dot, 0);
}


// dispNowTime
int sub_dispRtcTime(int x1, int x2, int x3, int y, int font, int dot, int Alldisp)
{
//#define PREV_HOUR

#ifdef PREV_HOUR
	static int prev_hour=-1;
#endif
	char str[32];

	util_uitoa_supply_char(*hour, str, 2, '0');
	emu_gv_kput(x1, y, str, font, dot, 0);
	emu_gv_kput(x2, y, ":", font, dot, 0);
	util_uitoa_supply_char(*min, str, 2, '0');
	emu_gv_kput(x3, y, str, font, dot, 0);
#ifdef PREV_HOUR
	if(prev_hour != *hour)
	{
		prev_hour=*hour;
		return 1;
	}
	return 0;
#endif

	return 1;
}


// dispNowSec
void sub_dispRtcSec(int x, int y, int font, int dot, int Alldisp)
{
	char str[32];

	util_uitoa_supply_char(*sec, str, 2, '0');
	emu_gv_kput(x, y, str, font, dot, 0);
}

