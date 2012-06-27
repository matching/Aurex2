/*
 * dispmess.c - dirpMESS... emulation code (for Mn10200 code)
 *
 * $Id: dispmess.c,v 1.4 2006/01/21 18:04:35 fumi Exp $
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



int sub_dispMESS(char **message, int row)
{
	int i;

	emu_gv_put(0, 0, (char *)mmpINFO, 0);

	for(i=0; i < row; i++)
		emu_gv_kput(2, 6+i*13, message[i], 0, 0, 0);

	emu_Keyin(0xff, -1);

	return 0;
}


int sub_dispMESS_us(char **message, int row)
{
	int i;

	emu_gv_put(0, 0, (char *)mmpINFO, 0);

	for(i=0; i < row; i++)
		emu_gv_kput2(2, 6+i*13, message[i], 5, 0, 0, 1);

	emu_Keyin(0xff, -1);

	return 0;
}
