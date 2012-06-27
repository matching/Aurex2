// -*-mode: c++; coding: euc-jp-unix;-*-
/*
 * rupsys.cpp - Rupsys interruption emulation
 *
 * $Id: rupsys.cpp,v 1.38 2006/02/18 08:45:09 fumi Exp $
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

#include <assert.h>

//#include "cpu.h"
#include "mncore.h"
#include "memory.h"
#include "memorycpu.h"
#include "thread.h"
#include "address.h"
#include "rupsys.h"
#include "ruputer.h"
#include "keybuffer.h"
#include "state.h"
#include "systimer.h"
#include "rupaddress.h"
#include <stdio.h>

#include "rupsysaddress.h"

#include "log.h"

static unsigned long systemCallAddress[21]={
	A_SYSCTL_00,
	A_SYSCTL_01,
	A_SYSCTL_02,
	A_SYSCTL_03,
	A_SYSCTL_04,
	A_SYSCTL_05,
	A_SYSCTL_06,
	A_SYSCTL_07,
	A_SYSCTL_08,
	A_SYSCTL_09,
	A_SYSCTL_0a,
	A_SYSCTL_0b,
	A_SYSCTL_0c,
	A_SYSCTL_0d,
	A_SYSCTL_0e,
	A_SYSCTL_0f,
	A_SYSCTL_10,
	A_SYSCTL_11,
	A_SYSCTL_12,
	A_SYSCTL_13,
	A_SYSCTL_14,
};

int SysIntr::initial_flag = 0;

SysIntr::SysIntr(Ruputer *ruputer, Thread *thread, unsigned long kernelsp)
{
	this->keybuffer = ruputer->getKeyBuffer();
	this->threadState = thread->getState();
	this->kernelsp = kernelsp;
	this->systemAddress = ruputer->getSystemAddress();
	this->ruputerState = ruputer->getState();

	if(initial_flag == 0)
	{
		initial_flag=1;
		initRupSysTable();
	}


	assert( keybuffer );
	assert( threadState );
	assert( systemAddress );
}

void SysIntr::initRupSysTable(void)
{
	int i;
	unsigned long offset;
	MemoryForInside *mem;

	mem=new MemoryForInside(A_SYSTEMCTL);

//	mem->write(A_SYSTEMCTL, 
//	p=(unsigned long *)(memory+A_SYSTEMCTL);

#ifdef DEBUG
	fprintf(stderr, "!!initializing  System Memory ....\n");
#endif

	offset=0;
	for(i=0; i < 0x15; i++)
	{
		mem->write24(offset, ::systemCallAddress[i]);
		offset+=4;
	}

	for(i=0x100; i <= 0x13b; i++)
	{
		mem->write24(offset, i);
		offset+=4;
	}
	delete mem;
	mem=new MemoryForInside();
/** onHand
 */
/* apisys */
	offset=0x234;
	mem->write24(offset, 0x1100);
	offset=0x1100;
	for(i=0x100; i <= 0x13b; i++)
	{
		mem->write24(offset, i);
		offset+=4;
	}

/* apimenu */
	offset=0x238;
	mem->write24(offset, 0x1200);
	offset=0x1200;
	for(i=0x200; i <= 0x220; i++)
	{
		mem->write24(offset, i);
		offset+=4;
	}

/* apitool */
	offset=0x23c;
	mem->write24(offset, 0x1300);
	offset=0x1300;
	for(i=0x300; i <= 0x33b; i++)
	{
		mem->write24(offset, i);
		offset+=4;
	}

	delete mem;
}


void SysIntr::callSysIntr(Register *regs, unsigned long addr)
{
	Log::getInstance()->output(Log::Debug, "Rupsys system call no.0x%lx\n", addr);

	switch(addr)
	{
	case 0x100: // Keyin
		int ret;
// キーバッファにあれば、即実行再開
		ret=keybuffer->emu_Keyin(regs);
		if(ret == 0)
			threadState->setState( ThreadState::WAIT_KEYIN );
		break;
	case 0x101: // Eventcall
		if( ruputerState->isEnding() )
			regs->d[0]=0xffffffUL;
		else
			regs->d[0]=0;

#ifdef DEBUG
		fprintf(stderr, "Eventcall return %x\n", regs->d[0] );
		fflush(stderr);
#endif

		break;
	case 0x102: //eventexec
		regs->d[0]=0;
		break;

	case 0x10f:
	case 0x110: // aprmenu
		callNativeCode(regs, A_EMU_APRMENU);
		break;

	case 0x10e: // dispmenu
	case 0x124: // dispmenuplusya
		callNativeCode(regs, A_EMU_DISPMENUPLUSYA);
		break;
	case 0x125: //timerstart
		systemAddress->set_timerstat(STS_RUN);
		break;
	case 0x126:// timerstop
		systemAddress->set_timerstat(STS_STOP);
		break;
	case 0x10b: //menuback
		regs->d[0]=0xfffffc;	/* -4 */
		break;
	case 0x10c: //aprend
		regs->d[0]=0xfffffe;	/* -2 */
		break;

	case 0x103: //softkey
		callNativeCode(regs, A_EMU_SOFTKEY);
		break;


	case 0x104: //dispmess
		callNativeCode(regs, A_EMU_DISPMESS);
		break;
	case 0x105: //dispynb
		callNativeCode(regs, A_EMU_DISPYNB);
		break;
	case 0x106: //dispyn
		callNativeCode(regs, A_EMU_DISPYN);
		break;
	case 0x107: //cancelCHK
		callNativeCode(regs, A_EMU_CANCELCHK);
		break;

	case 0x122: // endWAIT
		callNativeCode(regs, A_EMU_ENDWAIT);
		break;

	case 0x123: // dispWAIT
		callNativeCode(regs, A_EMU_DISPWAIT);
		break;

	case 0x129: // dispRtcWeek
		callNativeCode(regs, A_EMU_DISPRTCWEEK);
		break;
	case 0x12a: // dispRtcDate
		callNativeCode(regs, A_EMU_DISPRTCDATE);
		break;
	case 0x12b: // dispRtcAmPm
		callNativeCode(regs, A_EMU_DISPRTCAMPM);
		break;
	case 0x12c: // dispRtcTime
		callNativeCode(regs, A_EMU_DISPRTCTIME);
		break;
	case 0x12d: // dispRtcSec
		callNativeCode(regs, A_EMU_DISPRTCSEC);
		break;




/** onHand
 */
	case 0x12e: // aprMENUauto_USfont
		callNativeCode(regs, A_EMU_APRMENU_US);
		break;
	case 0x12f: //aprMENU_USfont
		callNativeCode(regs, A_EMU_APRMENU_US);
		break;
#if 0
	case 0x130: //settingMENU_USfont
		break;
#endif

	case 0x131: //dispTitle_USfont
		callNativeCode(regs, A_EMU_DISPMENUPLUSYA_US);
		break;
	case 0x132: //dispTitleMore_USfont
		callNativeCode(regs, A_EMU_DISPMENUPLUSYA_US);
		break;
	case 0x133: // dispMESS_USfont
		callNativeCode(regs, A_EMU_DISPMESS_US);
		break;
	case 0x134: // dispYNb_USfont
		callNativeCode(regs, A_EMU_DISPYNB_US);
		break;
	case 0x135: // dispYN_USfont
		callNativeCode(regs, A_EMU_DISPYN_US);
		break;

	case 0x13a:
/* +0xe8 13a	int (*getFilerVersion)(void);// Filer version */
		regs->d[0]=322;
		break;



/*********** ruptool *******************************************************/



	case 0x301: // StrCpy
		callNativeCode(regs, A_EMU_STRCPY);
		break;
	case 0x302: // StrnCmpI
		callNativeCode(regs, A_EMU_STRNCMPI);
		break;
	case 0x303: // strcat1
		callNativeCode(regs, A_EMU_STRCAT1);
		break;
	case 0x304: // strcat2
		callNativeCode(regs, A_EMU_STRCAT2);
		break;
	case 0x305: // strcpy1
		callNativeCode(regs, A_EMU_STRCPY1);
		break;
	case 0x306: // strcpy2
		callNativeCode(regs, A_EMU_STRCPY2);
		break;
	case 0x307: // strcpy3
		callNativeCode(regs, A_EMU_STRCPY3);
		break;
	case 0x308: // wstrcpy
		callNativeCode(regs, A_EMU_WSTRCPY);
		break;
	case 0x309: // strcpyadd0
		callNativeCode(regs, A_EMU_STRCPYADD0);
		break;
	case 0x30a: // strlength
		callNativeCode(regs, A_EMU_STRLENGTH);
		break;

/********************************* rupmenu ****************************/
/*rupmenu*/
	case 0x200: /* initInputTable */
		callNativeCode(regs, A_EMU_INITINPUTTABLE);
		break;
	case 0x201: /* endInputTable */
		callNativeCode(regs, A_EMU_ENDINPUTTABLE);
		break;
	case 0x202: /* entryItem */
		callNativeCode(regs, A_EMU_ENTRYITEM);
		break;
	case 0x204: /* entryItemMenu */
		callNativeCode(regs, A_EMU_ENTRYITEMMENU);
		break;
	case 0x207: /* setInputFont */
		callNativeCode(regs, A_EMU_SETINPUTFONT);
		break;

	case 0x209: /* setItemListToggle */
		callNativeCode(regs, A_EMU_SETITEMLISTTOGGLE);
		break;

	case 0x20a: /* setItemListVal */
		callNativeCode(regs, A_EMU_SETITEMLISTVAL);
		break;
	case 0x20b: /* getItemListVal */
		callNativeCode(regs, A_EMU_GETITEMLISTVAL);
		break;


	case 0x20e: /* getItemId */
		callNativeCode(regs, A_EMU_GETITEMID);
		break;

	case 0x214: /* setItemMenuNext */
		callNativeCode(regs, A_EMU_SETITEMMENUNEXT);
		break;

	case 0x219: /* runInputProc */
		callNativeCode(regs, A_EMU_RUNINPUTPROC);
		break;



	default:
		Log::getInstance()->output(Log::Warning, "Aurex doesn't support this rupsys system call no.0x%lx\n", addr);
		break;
	}

#if 0
	switch(addr)
	{
	case 0x108:
	/* void (*dispMSG)(char *)			 全角3文字ﾒｯｾｰｼﾞ表示				*/
		emu_dispMSG(FIXED_FONT);
		break;
	case 0x109:
	/* void (*dispKOUSIN)(void)			 "設定更新"表示				*/
		break;
	case 0x10a:
	/* void (*endKOUSIN)(void)			 "設定更新"終了				*/
		break;
/* ここまで */
	case 0x10d:
	/* int (*setKOUSIN)(void)			 設定の更新						*/
		regs.d[0]=0xfffffd;	/* -3 ? */
		break;
	case 0x111:
	/* int (*settingMENU)(char *,MENU *,int,int *,int)			 ﾒﾆｭｰ処理(内部関数)		*/
		emu_settingMenu(FIXED_FONT);
		break;
	case 0x112:
	/* void (*dispERR)(char *,ERRmess *)			 ｴﾗｰﾒｯｾｰｼﾞ表示					*/
	case 0x113:
	/* void (*dispERRnoBUZZ)(char *,ERRmess *)			 ｴﾗｰﾒｯｾｰｼﾞ表示(ﾌﾞｻﾞｰ無し)			*/
	case 0x114:
	/* void (*dispVOLerr)(void)			 ﾊﾞｯﾃﾘ異常表示					*/
	case 0x115:
	/* void (*dispMEMORYerr)(void)			 ﾒﾓﾘ異常表示						*/
	case 0x116:
	/* void (*dispBIOSerr)(void)			 BIOSｴﾗｰ表示						*/
	case 0x117:
	/* int (*watch_adj)(void)				時刻修正						*/
	case 0x118:
	/* int (*timeSEND)(void)			 時間の子機間転送(送信)			*/
	case 0x119:
	/* int (*timeRECV)(WatchDT *)			 時間の子機間転送(受信)			*/
	case 0x11a:
	/* void (*endTIMEAdj)(void)			 時間調整の終了					*/
	case 0x11b:
	/* void (*setRTC)(void)			 RTC設定							*/
		break;
	case 0x11c:
	/* void (*getRTC)(void)			 CPUramの時刻ﾃﾞｰﾀ更新				*/
		update_rtc();
		break;
	case 0x11d:
	/* int (*DfileSEND)(char *,int ,long )			 ｱﾌﾟﾘｹｰｼｮﾝ用ﾌｧｲﾙ転送-送信			*/
	case 0x11e:
	/* int (*DfileRECV)(char *,int )			 ｱﾌﾟﾘｹｰｼｮﾝ用ﾌｧｲﾙ転送-受信			*/
	case 0x11f:
	/* void (*setWeeklyAlarm)(void)			 週ｱﾗｰﾑ設定						*/
	case 0x120:
	/* void (*setAlarm)(void)			 ｲﾍﾞﾝﾄ･ｱﾗｰﾑ設定					*/
	case 0x121:
	/* void (*setPowerStandby)(void)			 ﾊﾟﾜｰｽﾀﾝﾊﾞｲ設定					*/
		break;
	case 0x127:
	/* int (*ChildExecute)(char *,int *)			 子ﾌﾟﾛｾｽの実行					*/
		emu_childexecute();
		break;
	case 0x128:
	/* void (*checkExecute)(void)			 子ﾌﾟﾛｾｽの実行					*/
		emu_checkexecute();
		break;
		break;
/***************************** onHand ***************************/
	case 0x12e:
/* +0xb8 12e	int (*aprMENUauto_USfont)*/

	case 0x12f:
// +0xbc 12f	int (*aprMENU_USfont)(char *,MENU *,int );/* Menu processing*/
		emu_aprMenu(PROP_FONT);
		break;
	case 0x130:
/* +0xc0 130	int (*settingMENU_USfont) */
		emu_settingMenu(PROP_FONT);
		break;
	case 0x131:
//  +0xc4 131 	void (*dispTitle_USfont)(char *);/* Display a menu name.*/
		emu_dispMENU(PROP_FONT);
		break;
	case 0x132:
/*  +0xc8 132	void (*dispTitleMore_USfont) */
		emu_dispMENUplusYa(PROP_FONT);
		break;
	case 0x133:
/* +0xcc 133	int (*dispMESS_USfont) */
		emu_dispMESS(PROP_FONT);
		break;
	case 0x134:
/* +0xd0 134	int (*dispYNb_USfont) */
		emu_dispYNb(PROP_FONT);
		break;
	case 0x135:
/* +0xd4 135	int (*dispYN_USfont) */
		emu_dispYN(PROP_FONT);
		break;
	case 0x136:
/* +0xd8 136	void (*dispMSG_USfont)*/
		emu_dispMSG(PROP_FONT);
		break;
	case 0x137:
/* +0xdc 137	int (*dispERR_USfont) */
	case 0x138:
/*  +0xe0 138	void (*setSaver)(void); */
	case 0x139:
/*  +0xe4 139	int (*execShortcut)(char *exename); */
		break;
	case 0x13a:
/* +0xe8 13a	int (*getFilerVersion)(void);// Filer version */
		emu_getFilerVersion();
		break;
	case 0x13b:
/* +0xec 13b	int (*pauseScroll2)(int,int,int,int);// Jump mark*/
		break;
/********************************* rupmenu ****************************/
/*rupmenu*/
	case 0x200:
/*	void (*initInputTable)(char *,INPUTITEM *);*/
		emu_initInputTable();
		break;
	case 0x201:
/*	void (*endInputTable)(void);*/
		emu_endInputTable();
		break;
	case 0x202:
/*	int  (*entryItem)(char *,char *,int); */
		emu_entryItem();
		break;
	case 0x203:
/*	int  (*entryItemEx)(char *, int *); */
		break;
	case 0x204:
/*	int  (*entryItemMenu)(char *,int *);*/
		emu_entryItemMenu();
		break;
	case 0x205:
/*	int  (*entryItemExtInfo)(int,char *,char *,int *,int *); */
	case 0x206:
/*	int  (*setInitialItemId)(int); */
		break;
	case 0x207:
//	void (*setInputFont)(int);		/* You should set prior to entryItem(). 	*/
		emu_setInputFont();
		break;
	case 0x208:
/*	void (*setChfiler)(int); */
	case 0x209:
/*	int (*setItemListToggle)(int,int); */
		break;
	case 0x20a:
/*	int (*setItemListVal)(int,int); */
		emu_setItemListVal();
		break;
	case 0x20b:
/*	int (*getItemListVal)(int); */
		emu_getItemListVal();
		break;
	case 0x20c:
/*	char *(*getItemListStr)(int,int); */
	case 0x20d:
/*	INPUTITEM *(*getInputItemNow)(void); */
		break;
	case 0x20e:
/*	int (*getItemId)(char *); */
		emu_getItemId();
		break;
	case 0x20f:
/*	int (*getLastItemId)(void); */
	case 0x210:
/*	int (*getLastKeyState)(void); */
	case 0x211:
/*	void (*arrangeItemListX)(int,int); */
	case 0x212:
/*	int (*setItemExtInfoData)(int,char *); */
	case 0x213:
/*	char *(*getItemExtInfoData)(int); */
		break;
	case 0x214:
/*	int (*setItemMenuNext)(int,int); */
		emu_setItemMenuNext();
		break;
	case 0x215:
/*	int (*setItemSelectMode)(int,int); */
		emu_setItemSelectMode();
		break;
	case 0x216:
/*	int (*setItemDisable)(int,int); */
	case 0x217:
/*	int (*setItemCallBack)(int,void *); */
	case 0x218:
/*	void (*dispItemRW)(int); */
		break;
	case 0x219:
/*	int (*runInputProc)(void);*/
		emu_runInputProc();
		break;
	case 0x21a:
/*	int (*retToggle)(int,int); */
	case 0x21b:
/*	int (*retToggleBoth)(int,int,int); */
	case 0x21c:
/*	int (*retNonToggle)(int,int);*/
	case 0x21d:
/*	int (*retNonToggleBoth)(int,int,int); */
	case 0x21e:
/*	int (*entryItemTime)(char *,INPUTTIME *); */
	case 0x21f:
/*	void (*setDblSignal)(int); */
	case 0x220:
/*	void (*resetKeyflg)(void); */
		break;
/*********** ruptool *******************************************************/
	case 0x300:
/*	int  (*getUsWdot)(int, char *); */
/* 必要無い */
		emu_getUsWdot();
		break;
	case 0x301:
/*	void (*StrCpy)(char *,const char *);*/
		emu_StrCpy();
		break;
	case 0x302:
/*	int  (*StrnCmpI)(char * ,char * ,int );*/
		emu_StrnCmpI();
		break;
	case 0x303:
/*	void (*strcat1)(char *,const char *);*/
		emu_strcat1();
		break;
	case 0x304:
/*	void (*strcat2)(char *,const char *,const char *);*/
		emu_strcat2();
		break;
	case 0x305:
/*	void (*strcpy1)(char *,const char *);*/
		emu_strcpy1();
		break;
	case 0x306:
/*	void (*strcpy2)(char *,const char *,const char *);*/
		emu_strcpy2();
		break;
	case 0x307:
/*	void (*strcpy3)(char *,const char *,const char *,const char *);*/
		emu_strcpy3();
		break;
	case 0x308:
/*	void (*wstrcpy)(char *,const char *,char *, const char *);*/
		emu_wstrcpy();
		break;
	case 0x309:
/*	void (*strcpyadd0)(char *,const char *,int);*/
		emu_strcpyadd0();
		break;
	case 0x30a:
/*	int  (*strlength)(const char *); */
		emu_strlength();
		break;
	case 0x30b:
/*	void (*SetExitfunc)( void (*)( void ) );		//Set a function to invoke when dos_exit() is requested. */
	case 0x30c:
/*	void (*StartApr)( void (*)( void ) );			//Initialize application.*/
	case 0x30d:
/*	int (*EventChk)(int );							//Event check*/
	case 0x30e:

// ファイル関係の関数は呼び出し不可の感じがする。
// なので、とりあえず作るきはしない。
		
/*	int (*FileCreate)(char *);						//Create a file. */
	case 0x30f:
/*	int (*NewFileOpen)(char *);						//Create a new file.*/
	case 0x310:
/*	int (*FileOpen)(char *,int );					//Open a file.*/
	case 0x311:
/*	unsigned long (*FileSeek)(int ,int ,unsigned long );//Seek a file.*/
	case 0x312:
/*	void (*FileWrite)(int ,char *,unsigned int );	//Write to a file.*/
	case 0x313:
/* 	void (*WriteSpace)(long );						//Judge a remaining space. */
	case 0x314:
/* 	int (*FileRead)(int ,char *,unsigned int );		//Read from a file. */
	case 0x315:
/* 	void (*FileClose)(int );						//Close a file. */
	case 0x316:
/* 	void (*FileFlash)(int );						//Store into flash. */
	case 0x317:
/*	void (*FileDelete)(char *);						//Delete a file.*/
	case 0x318:
/* 	void (*FileSystemAttr)( int, char * );			//Change a file attribute to system file attribute */
	case 0x319:


/* 	void (*INTtoASC1)(char *,int ,int ,int );		//Change a digit to a character string(with \0 for scosii code). */
	case 0x31a:
/* 	void (*INTtoASC2)(char *,int ,int ,int );		//Change a digit to a character string (without \0 for scosii code). */
	case 0x31b:
/* 	void (*INTtoASCcode0)(char *,int ,int ,int );	//Change a digit to a character string (without \0 for normal code). */
	case 0x31c:
/* 	int (*IsKanji_1)( unsigned char *, int );		//Judge whether a specified character is full-size second byte.  */
	case 0x31d:
/* 	int (*IsKanji)( unsigned char *,int );			//Judge whether a specified character is a full size.  */
	case 0x31e:
/* 	int (*CheckFileName)( char *, char *, char * );	//Judge with an extention whether a file is proper. */
	case 0x31f:
/* 	unsigned long (*GetDateID_in)( void );			//Obtain a unique ID. */
	case 0x320:
/* 	unsigned long (*GetDateID)( void );				//Obtain a unique ID(corresponding to PIMLINK, synchro flag). */
	case 0x321:
/* 	int (*CheckLeapYear)( int );					//Judge whether it is a leap year. */
	case 0x322:
/* 	int (*CheckAmPm)( int * );						//Judge whether it is AM/PM. */

	case 0x323:
/* 	void (*CheckToday)( TypeDate * );				//Obtain a date of today.(on RAM) */

	case 0x324:
/* 	int (*CheckWeek)( int,int,int );				//Obtain a day of the week. */

	case 0x325:
/* 	int (*Get2WeekMonday)( int , int );				//Obtain a date of the second Monday. */
	case 0x326:
/* 	int (*CheckSecret)( int );						//Judge secret. */

	case 0x327:
/* 	void (*SetMessBuf)( char *, char * );			//Create a message asking Do you **? */
	case 0x328:
/* 	int (*KeyWait)( void );							//Key waiting function */
	case 0x329:
/* 	int (*dispVersion)( char *[], int , int );		//Display version information. */
	case 0x32a:
/* 	void (*DeleteTimer)( int * );					//Delete timer. */
	case 0x32b:
/* 	int (*GetNWeek)( int, int, int, int );			//Obtain a date of nth day of the week. */
	case 0x32c:
/* 	int (*GetSndFileName)( char *, char [][9], int );//Obtain a name of a sound file. */
	case 0x32d:
/* 	void (*SoundDEMO)( char * );					//sound demo */

	case 0x32e:
/* 	void (*GetNdaysafter)( TypeDate *, long , int );//Obtain a date after a few days. */
	case 0x32f:
/* 	int (*YearDayCount)( int );						//Obtain the total number of days of a year. */
	case 0x330:
/* 	int (*CheckDayCountSet)( TypeDate * );			//Obtain information some days after January 1.  */
	case 0x331:
/* 	long (*GetSubDays)( TypeDate *, TypeDate * );	//Obtain a difference between one date and another date.  */
	case 0x332:
/* 	int (*MINtoHOURMIN)( TypeTime *, int );			//Change time  (a value converted in minutes) into hour and minute. */
	case 0x333:
/* 	void (*HOURMINtoMIN)( int *, TypeTime * );		//Change time (hour and minute) to a value converted in minutes.  */
	case 0x334:
/* 	int (*GetSubTime)( TypeTime *, TypeTime * );	//Obtain a difference between time and time. */
	case 0x335:
/* 	int (*GetNhourafter)( TypeTime *, int );		//Obtain time after N minutes.  */
	case 0x336:
/* 	int (*CmpDay)( TypeDate *, TypeDate * );		//Compare dates in size. */
	case 0x337:
/* 	int (*CmpTime)( TypeTime *, TypeTime * );		//Compare time in size. */
	case 0x338:
/* 	void (*IncDate)( TypeDate *, int , int );		//+one day */
	case 0x339:
/* 	void (*DecDate)( TypeDate *, int , int );		//-one day */
	case 0x33a:
/* 	int (*CheckTimeTable)( TypeDate *, TypeDate *, TypeDate * );//Judge whether a date is within a range. */
	case 0x33b:
/* 	void (*SetAlarmIni)( char *, char * );			//Update Alarm.ini. */

	break;
	}
#endif
}


void SysIntr::callNativeCode(Register *regs, unsigned long addr)
{
	MemoryForInside mem;

	regs->a[3]-=8;
	mem.write24(regs->a[3], kernelsp);
	mem.write24(regs->a[3]+4, regs->pc);

#ifdef DEBUG
	fprintf(stderr, "rupsys call kernel sp 0x%lx, addr\n", kernelsp, addr);
#endif

	regs->pc=addr;
}


