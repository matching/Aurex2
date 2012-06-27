/*
 * lcd.cpp - LCD Interruption (System call) emulation code
 *
 * $Id: lcd.cpp,v 1.20 2006/01/21 18:04:34 fumi Exp $
 *
 * Copyright (C) 2004, 2005, 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

/*
 * LCD interrupt
 *
 * 呼び出しの性質上、各構造体中のポインタは 24 bit 化する必要あり
 *
 */


#include <stdio.h>
#include <assert.h>


#include "mncore.h"
#include "lcd.h"
#include "lcdfunc.h"
#include "ruputer.h"

#include "log.h"

/**
 * LCD 割り込みのクラス
 * @param ruputer Ruputerクラスのinstance
 */
LcdIntr::LcdIntr(Ruputer *ruputer)
{
	cmlcd = ruputer->getCMLcd();
	systemAddress = ruputer->getSystemAddress();

	assert( cmlcd );
	assert( systemAddress );
}

/**
 * LCD 割り込み呼び出し
 * @param regs 割り込み入れた時のレジスタ値
 * @retval なし
 */
void LcdIntr::callLcdFunc(Register *regs)
{
	LcdFunc *lcdFunc=NULL;

	int func_no;
	unsigned long res=0;
	MemoryForInside *mem;


	mem=new MemoryForInside(regs->a[0]);

	func_no=mem->read16_u(0);

	Log::getInstance()->output(Log::Debug, "LCD interrupt no. %d\n", func_no);

	switch(func_no)
	{
	case 0:
		lcdFunc=new LcdFuncScreen(cmlcd, mem);
		break;
	case 2:
		lcdFunc=new LcdFuncCls(cmlcd, mem);
		break;

	case 7:
		lcdFunc=new LcdFuncGetbufsiz(cmlcd, mem);
		break;
// reverse
	case 8:
		lcdFunc=new LcdFuncReverse(cmlcd, mem);
		break;
	case 9:
		lcdFunc=new LcdFuncRevstart(cmlcd, mem);
		break;
	case 10:
		lcdFunc=new LcdFuncRevstop(cmlcd, mem);
		break;
	case 11:
		lcdFunc=new LcdFuncRevend(cmlcd, mem);
		break;
#if 0
	case 12:
		res=emu_revdraw();
		break;
#endif

// blink
	case 13:
		lcdFunc=new LcdFuncBlink(cmlcd, mem);
		break;

	case 14:
		lcdFunc=new LcdFuncBlstop(cmlcd, mem);
		break;
	case 15:
		lcdFunc=new LcdFuncBlstart(cmlcd, mem);
		break;
	case 16:
		lcdFunc=new LcdFuncBlend(cmlcd, mem);
		break;

	case 17:
		lcdFunc = new LcdFuncLcdfreeze(cmlcd, mem);
		break;


	case 60:
		lcdFunc=new LcdFuncGv_place(cmlcd, mem, systemAddress);
		break;

	case 20:
	case 61:
		lcdFunc=new LcdFuncGv_kput(cmlcd, mem);
		break;
	case 21:
	case 62:
		lcdFunc=new LcdFuncGv_sput(cmlcd, mem);
		break;

	case 22:
	case 63:
		lcdFunc=new LcdFuncGv_aput(cmlcd, mem);
		break;
	case 24:
	case 65:
		lcdFunc=new LcdFuncGv_line(cmlcd, mem);
		break;
	case 25:
	case 66:
		lcdFunc=new LcdFuncGv_square(cmlcd, mem);
		break;
	case 26:
	case 67:
		lcdFunc=new LcdFuncGv_circle(cmlcd, mem);
		break;
	case 27:
	case 68:
		lcdFunc = new LcdFuncGv_pset(cmlcd, mem);
		break;
	case 28:
	case 69:
		lcdFunc = new LcdFuncGv_point(cmlcd, mem);
		break;
	case 29:
	case 70:
		lcdFunc=new LcdFuncGv_get(cmlcd, mem);
		break;
	case 30:
	case 71:
		lcdFunc=new LcdFuncGv_put(cmlcd, mem);
		break;

	case 31:
	case 73:
		lcdFunc=new LcdFuncGv_reverse(cmlcd, mem);
		break;
	case 32:
	case 74:
		lcdFunc=new LcdFuncGv_clear(cmlcd, mem);
		break;

/* 自作（native から呼ばれる） */
	case 161:
		lcdFunc=new LcdFuncGv_kput2(cmlcd, mem);
		break;


#if 0
	case 0:
		res=emu_screen();
		break;
	case 1:
		res=emu_getscreen();
		break;
	case 3:
		res=emu_lcdon();
		break;
	case 4:
		res=emu_getlcdon();
		break;
	case 6:
		res=emu_mojisize();
		break;
	case 7:
		res=emu_getbufsize();
		break;
	case 8:
		res=emu_reverse();
		break;
	case 9:
		res=emu_revstart();
		break;
	case 10:
		res=emu_revstop();
		break;
	case 11:
		res=emu_revend();
		break;
	case 12:
		res=emu_revdraw();
		break;
	case 13:
		res=emu_blink();
		break;
	case 14:
		res=emu_blstop();
		break;
	case 15:
		res=emu_blstart();
		break;
	case 16:
		res=emu_blend();
		break;
	case 17:
		res=emu_lcdfreeze();
		break;
#if 0
#define		FNO_VOL				18					/* ＬＣＤ電子ボリューム量設定	*/
#define		FNO_GETVOL			19					/* ＬＣＤ電子ボリューム量取得	*/

#endif
	case 20:
	case 61:
		res=emu_gv_kput();
		break;
	case 21:
	case 62:
		res=emu_gv_sput();
		break;
	case 22:
	case 63:
		res=emu_gv_aput();
		break;
	case 24:
	case 65:
		res=emu_gv_line();
		break;
	case 25:
	case 66:
		res=emu_gv_square();
		break;
	case 26:
	case 67:
		res=emu_gv_circle();
		break;
	case 27:
	case 68:
		res=emu_gv_pset();
		break;
	case 28:
	case 69:
		res=emu_gv_point();
		break;
	case 29:
	case 70:
		res=emu_gv_get();
		break;
	case 30:
	case 71:
		res=emu_gv_put();
		break;
	case 31:
	case 73:
		res=emu_gv_reverse();
		break;
	case 60:
		res=emu_gv_place();
		break;
	case 72:
		res=emu_gv_scroll();
		break;
#endif
	default:
		Log::getInstance()->output(Log::Warning, "Aurex doesn't support this LCD interrupt no.%d\n", func_no);
		break;
	}



	if(lcdFunc != NULL)
		res=lcdFunc->callLcdFunc();



/* res の処理 */
	regs->d[0]=res & 0xffff;
	regs->d[1]=res >> 16;

	delete mem;
	delete lcdFunc;
}


