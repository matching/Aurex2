/*
 * dispwait.c - dispwait, endwait emulation code (for Mn10200 code)
 *
 * $Id: dispwait.c,v 1.2 2005/01/10 08:40:27 fumi Exp $
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


#include "emu_lcdbios.h"
#include "emu_wbios.h"
#include "emu_font.h"
#include "itoa.h"

static void waitfunc(void);

static int *waitTimerId = 0;


static void waitfunc(void)
{
	static int index = 0;
	static void const *mmp_dolphin[0xb] = {	mmpDR1,
											mmpDR2,
											mmpDR3,
											mmpDR4,
											mmpDR5,
											mmpDR6,
											mmpDR7,
											mmpDR8,
											mmpDR9,
											mmpDRa,
											mmpDRb,};

	emu_gv_put( 40, 20, mmp_dolphin[ index ], 0 );
	index++;
	if( index >= 0xb )
		index = 0;
}


void sub_dispWAIT(void)
{
	emu_gv_clear(0, 0, 102, 64);

	waitTimerId = emu_sys_bi_tmset(1, 255, 2, waitfunc);
}

void sub_endWAIT(void)
{
	emu_sys_bi_tmdel( waitTimerId );
	waitTimerId = 0;
}

