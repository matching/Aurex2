/*
 * interruption.cpp -
 *
 * $Id: interruption.cpp,v 1.12 2004/10/31 13:41:18 fumi Exp $
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

#include "address.h"
#include "interruption.h"
#include "memory.h"
#include "ruputer.h"

#include "cpu.h"
#include "rupsys.h"
#include "psdos.h"
#include "wbios.h"
#include "lcd.h"


Interruption::Interruption(DosIntr *dosIntr, LcdIntr *lcdIntr, BiosIntr *biosIntr, SysIntr *sysIntr)
{
	this->dosIntr = dosIntr;
	this->lcdIntr = lcdIntr;
	this->biosIntr = biosIntr;
	this->sysIntr = sysIntr;
}

Interruption::~Interruption(void)
{
	delete dosIntr;
	delete lcdIntr;
	delete biosIntr;
	delete sysIntr;
}

/*
 * reg_an は、jsr (an) の呼び出した a レジスタの番号
 */
int Interruption::execSystemCall(Register *regs, unsigned long *areg)
{
	if(*areg < 0x400)
	{
		sysIntr->callSysIntr(regs, *areg);	/* Softkey, aprMenu の様な呼び出し */
		return 2;
	}
#if 0
	else if(*areg >= 0x800000)
	{
// native call
		if(*areg <  0x810000)
			lcdIntr->callNativeLcdFunc(*areg);
//		else if(*areg < 0x820000)
	}
#endif
	else if(*areg == A_SWINTENT)
	{
		switch(regs->d[0] & 0xff)
		{
		case 20: /* dos 割り込み */
			dosIntr->callDosIntr(regs);
			break;
		case 22: /* lcd bios の割り込み */
			lcdIntr->callLcdFunc(regs);
			break;
		case 23: /* 0x17 ボタン入力(KEY入力) （割り込み番号 23）*/
		case 24: /* 0x18 ディスク制御        （割り込み番号 24) */
		case 25: /* 0x19 ＲＳ−２３２Ｃ，Ｉｒ（割り込み番号 25）*/
		case 26: /* 0x1a ＲＴＣ              （割り込み番号 26) */
		case 28: /* 0x1c タイマ              （割り込み番号 28) */
		case 29: /* 0x1d その他              （割り込み番号 29) */
			biosIntr->callBiosIntr(regs);

			break;
		}

		return 1;
	}

	return 0;
}


