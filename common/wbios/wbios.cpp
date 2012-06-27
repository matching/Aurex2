/*
 * wbios.cpp - Ruputer wbios emulation
 *
 * $Id: wbios.cpp,v 1.24 2006/02/17 19:01:38 fumi Exp $
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


#include <stdio.h>
#include <assert.h>

#include "cpu.h"
#include "wbios.h"
#include "ruputer.h"
#include "timer.h"
#include "timermgr.h"
#include "state.h"
#include "keybuffer.h"
#include "thread.h"
#include "rupstruct.h"

#include "log.h"

BiosIntr::BiosIntr(Ruputer *ruputer, Thread *thread)
{
	volume=0;
	tempo=0;

	this->threadState = thread->getState();
	this->thread = thread;
	this->ruputer = ruputer;
	this->keybuf = ruputer->getKeyBuffer();
	this->timermgr = ruputer->getTimerMgr();
	this->systimermgr = ruputer->getSysTimerMgr();
	this->watchDT = ruputer->getWatchDT();
	this->threadList = ruputer->getThreadList();


	assert( thread );
	assert( threadState );
	assert( keybuf );
	assert( timermgr );
	assert( systimermgr );
	assert( watchDT );
//	assert( sdSound );
}

BiosIntr::~BiosIntr(void)
{
// do nothing
}

void BiosIntr::callBiosIntr(Register *regs)
{
	Log::getInstance()->output(Log::Debug, "BIOS interrupt no.0x%lx\n", regs->d[0]);

// todo 各メソッドに regs を引数に持たせる
	this->regs = regs;

	switch(regs->d[0])
	{
	case 0x117:	/* bi_getbtn */
// キーバッファにあれば、即実行再開
		int ret;
		ret=keybuf->emu_bi_getbtn(regs);
		if(ret == 0)
			threadState->setState( ThreadState::WAIT_BI );
		break;
	case 0x217: /* bi_sensbtn */
		keybuf->emu_bi_sensbtn(regs);
		break;

	case 0xb18:	/* bi_wrflush */
		regs->d[1]=0;	/* 正常終了 */
		break;

	case 0x11c:	/* bi_tmset */
		bi_tmset();
		break;
	case 0x21c: /* bi_tmdel */
		bi_tmdel();
		break;

	case 0x11a:	/* bi_tmread */
		bi_tmread();
		break;
	case 0x31a: /* bi_dtread */
		bi_dtread();
		break;

	case 0x51d:  /* bi_powerread */
	case 0x101d: /* bi_powerread2 */
		regs->d[1]=135;	/* 6 V just */
		break;

	case 0x111d:	/* 27Verror 電圧チェック */
		regs->d[1]=0;
		break;

	case 0x81d: /* bi_getpower */
#define FULL_BATT 160000000
		regs->d[2]=FULL_BATT >> 16;
		regs->d[1]=FULL_BATT & 0xffff;
		break;


	case 0xb1d:
		bi_buzzinit();
		break;
	case 0xe1d:
		bi_melody();
		break;
	case 0x21d:
		bi_buzzer();
		break;

	case 0x01011c:
		sys_bi_tmset();
		break;
	case 0x01021c: /* bi_tmdel */
		sys_bi_tmdel();
		break;

	}


#if 0
	case 0x21a: /* bi_tmwrite */
	case 0x41a: /* bi_dtwrite */
		regs.d[1]=0;
		break;


	case 0xf1d:
		emu_bi_canmelody();
		break;



	case 0x317: /* bi_clrbtn */
	case 0x417: /* bi_chapsbtn */
	case 0x517:	/* bi_chatjktbtn */
	case 0x118:	/* bi_secread */
	case 0x218:	/* bi_secwrite */
	case 0x318:	/* bi_format */
	case 0x418: /* bi_getdrform */
	case 0x518:	/* bi_setdrform */
		break;
	case 0xd18: /* bi_sysread */
		emu_bi_sysread();
		break;
	case 0xe18:	/* bi_syswrite */
		break;
	case 0x119:	/* bi_cominit */
		emu_bi_cominit();
		break;
	case 0x219:	/* bi_comwrite */
		emu_bi_comwrite();
		break;
	case 0x319:	/* bi_comread */
		emu_bi_comread();
		break;
	case 0x419:	/* bi_complread */
		emu_bi_complread();
		break;
	case 0x519: /* bi_comclr */
		emu_bi_comclr();
		break;
	case 0x619: /* bi_comrnum */
		emu_bi_comrnum();
		break;
	case 0x719: /* bi_comopen */
		emu_bi_comopen();
		break;
	case 0x819: /* bi_comclose */
		emu_bi_comclose();
		break;
	case 0x919: /* bi_complwrite */
		emu_bi_complwrite();
		break;
	case 0x11d: /* bi_reboot */
	case 0x31d: /* bi_cpususp */
	case 0x41d: /* bi_cpuspeed */
	case 0x61d: /* bi_getcharge */
	case 0x71d: /* bi_clrcharge */
	case 0x91d: /* bi_powerinc */
		break;
	case 0xa1d: /* bi_el */
		break;
	case 0xc1d: /* bi_current */
	case 0xd1d: /* bi_setpower */
		break;

	default:	/* 割り込みベクトル云々 */
//		printf("get/set interrupt vector\n");
		Log::getInstance()->output(Log::Warning, "Aurex doesn't support this WBios intterupt no.0x%lx\n", regs->d[0]);
		break;
	}

#endif



}




/*
 * d1: priority 上位 8
 * d1:          下位 8 bit でタイマ種別
 * d2: タイマカウント
 * a1: タイマルーチンのアドレス
 *
 * a0: タイマ id へのポインタ
 */
void BiosIntr::bi_tmset(void)
{
	int type;

	type=regs->d[1] & 0xf;

	switch(type)
	{
	case 0:	/* 1 hz timer */
		regs->a[0]=timermgr->create1HzTimer(regs->a[1], regs->d[2]);
		break;
	case 1: /* 4 Hz timer */
		regs->a[0]=timermgr->create4HzTimer(regs->a[1], regs->d[2]);
		break;
	case 2: /* 64 Hz timer */
		regs->a[0]=timermgr->create64HzTimer(regs->a[1], regs->d[2]);
		break;
	}

#ifdef DEBUG
	fprintf(stderr, "bi_tmset retrun %x %d \n", regs->a[0], regs->d[2]);
#endif


	if(regs->a[0] == 0)
		regs->d[1]=1;
	else
		regs->d[1]=0;
}


/* a0 timer id
 * d0 return value
 */
void BiosIntr::bi_tmdel(void)
{
/* 標準テキストビュアーで a0 に -1(0xffffff) がくることがある。
 */
	timermgr->deleteTimer(regs->a[0]);
	regs->d[1]=0;
}


/*
 * d1 : 0
 * d2 : 時
 * d3 : 分
 * a1 : 秒
 */
void BiosIntr::bi_tmread(void)
{
	regs->d[1]=0;
	regs->d[2]=watchDT->getHourBcd();
	regs->d[3]=watchDT->getMinBcd();
	regs->a[1]=watchDT->getSecBcd();
}

/*
 * d1 : 0 正常時
 * d2 :  年
 * d3 :  月
 * a1 :  日
 */
void BiosIntr::bi_dtread(void)
{
	regs->d[1]=0;
	regs->d[2]=watchDT->getYearBcd();
	regs->d[3]=watchDT->getMonthBcd();
	regs->a[1]=watchDT->getMDayBcd();
}






/***/

/*
 * d1: priority 上位 8
 * d1:          下位 8 bit でタイマ種別
 * d2: タイマカウント
 * a1: タイマルーチンのアドレス
 *
 * a0: タイマ id へのポインタ
 */
void BiosIntr::sys_bi_tmset(void)
{
#ifdef DEBUG
	char str[1024];

	regs->toString(str);
	fprintf(stderr, "- sys_bi_tmpset -----------------------\n%s\n--------------------------\n", str);
	fflush(stderr);
#endif

	int type;

	type=regs->d[1] & 0xf;

	switch(type)
	{
	case 0:	/* 1 hz timer */
		regs->a[0]=systimermgr->create1HzTimer(regs->a[1], regs->d[2]);
		break;
	case 1: /* 4 Hz timer */
		regs->a[0]=systimermgr->create4HzTimer(regs->a[1], regs->d[2]);
		break;
	case 2: /* 64 Hz timer */
		regs->a[0]=systimermgr->create64HzTimer(regs->a[1], regs->d[2]);
		break;
	}


#ifdef DEBUG
	fprintf(stderr, "sys_bi_tmset retrun 0x%x\n", regs->a[0]);
	fflush(stderr);
#endif


	if(regs->a[0] == 0)
		regs->d[1]=1;
	else
		regs->d[1]=0;
}


/* a0 timer id
 * d0 return value
 */
void BiosIntr::sys_bi_tmdel(void)
{
#ifdef DEBUG
	fprintf(stderr, "sys_bi_tmdel arg is 0x%x\n", regs->a[0]);
	fflush(stderr);
#endif

/* 標準テキストビュアーで a0 に -1(0xffffff) がくることがある。
 */
	systimermgr->deleteTimer(regs->a[0]);
	regs->d[1]=0;
}
