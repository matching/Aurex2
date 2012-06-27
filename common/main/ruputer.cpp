/*
 * ruputer.cpp - Ruputer emulation main
 *
 * $Id: ruputer.cpp,v 1.92 2008/07/26 06:43:08 fumi Exp $
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


#include "ruputer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef unix
#include <unistd.h>
#else
#include <direct.h>
#endif

#include <assert.h>

#include <list>
#include <iterator>
#include <string>
#include <map>
using namespace std;

#include "cpu.h"
#include "frame.h"
#include "cmlcd.h"
#include "sdlcd.h"
#include "memory.h"
#include "memorycpu.h"
#include "thread.h"
#include "state.h"
#include "timer.h"
#include "timermgr.h"
#include "rupaddress.h"
#include "keybuffer.h"
#include "rupstruct.h"
#include "systimer.h"
#include "exmemory.h"
#include "option.h"

#include "blinkmgr.h"
#include "reversemgr.h"
#include "lcdblrvmgr.h"
#include "directory.h"
#include "filerini.h"
#include "ufilerini.h"
#include "filerinimgr.h"
#include "ruputerdirectory.h"
#include "directoryconverter.h"
#include "rommmp.h"
#include "port.h"
#include "internalfont.h"

#include "exffile.h"
#include "log.h"
#include "logoutput.h"
#include "logoutputfile.h"
#include "logoutputstderr.h"
/*
KeyBuffer *Ruputer::keybuf=NULL;
Ruputer *ruputer;
*/


/** コンストラクタ
 * @param frame 時間配分を司る Frame クラス（を継承したクラス）
 *
 * @param sddir 仮想ルートディレクトリを保持するクラス。Ruputer 内で 
 * new しないのは、実行環境が Windows か Unix かを呼び出す方で決めても
 * らうため。
 * @param sdsound SDSoundオブジェクト。システム依存の音鳴らしクラス
 */
Ruputer::Ruputer(Frame *frame, Directory *sddir, Option *option, UFilerIni *user_ini)
{
	::option = option;

	this->localDirectory = sddir;
	this->frame = frame;

	this->cpuspeed = 100;
	this->framespeed = 0;

	this->dta_address = 0x1020;

	this->cmlcd = new CMLcd(this);
	this->ruputerDirectory = new RuputerDirectory(sddir);
	this->systemAddress = new RupAddress();
	this->keybuf = new KeyBuffer(systemAddress, &ruputerState);
	this->systimer = new SysTimer(systemAddress, keybuf);
	this->timermgr    = new TimerMgr(this, 2, 1, 1); // 1Hz: 2, 4Hz:1, 64Hz:1
	this->systimermgr = new TimerMgr(this, 4, 5, 3); // 1Hz: 4, 4Hz:5, 64Hz:3
	this->watchdt = new WatchDT();

	this->newLoadFileName = NULL;
	this->mainThread = NULL;
	this->childThread = NULL;

	ExMemory _dummy; // constructor だけで処理終了

	filerini=new FilerIniMgr(NULL, user_ini);
	memoryForCpu = new MemoryForCpu(new Port(cmlcd, this),
									new InternalFont(this),
									new RomMmp());

	this->exffile = NULL;

	setupLog();

	ruputerState.setState( RuputerState::STOP);
	mainThreadState.setState( ThreadState::EXIT );

}

/** デストラクタ
 */
Ruputer::~Ruputer(void)
{
	delete cmlcd;
	delete ruputerDirectory;
	delete systemAddress;
	delete keybuf;
	delete systimer;
	delete timermgr;
	delete systimermgr;
	delete watchdt;
	delete filerini;

	delete memoryForCpu;

	delete exffile;

	closeAllThread();
}

/* #define CYCLE_FOR_64HZ (920000/64/4) */
#define CYCLE_FOR_64HZ (920000/64)

/** メインループ
 */
int Ruputer::main_loop(void)
{
	int cycle;

	frame->startFrame();
	for(;;)
	{
		if(EventProcedure())
			continue;

#ifdef DEBUG
		fprintf(stderr, "ruputerState: %d, mainthreadstate:%d\n", ruputerState.getState(), mainThreadState.getState());
		fflush(stderr);
#endif
		switch(ruputerState.getState())
		{
		case RuputerState::STOP:
			frame->wait();
			continue;
		case RuputerState::RUNNING:
			if( mainThreadState.isRunning() )
			{
				break; // do nothing
			}
			ruputerState.setState( RuputerState::STOP );
			continue;

		case RuputerState::ENDING_RELOAD:
			if( ! mainThreadState.isRunning() || ruputerState.isEndingCountOver() )
			{
				deleteMainThread();
				reload();
				continue;
			}
			break;
		case RuputerState::ENDING_NEW_LOAD:
			if( ! mainThreadState.isRunning() || ruputerState.isEndingCountOver() )
			{
				if( mainThreadState.isRunning() )
					Log::getInstance()->output(Log::Warning, "Warning. The program is still running. but Aurex load new file.\n");

				assert( newLoadFileName );
				deleteMainThread();
				init();
				load( newLoadFileName );
				delete newLoadFileName;
				newLoadFileName = NULL;

				break;
			}
			break;
		case RuputerState::ENDING_DESTROY:
			if( ! mainThreadState.isRunning() || ruputerState.isEndingCountOver() )
			{
				if( mainThreadState.isRunning() )
					Log::getInstance()->output(Log::Warning, "Warning. The program is still running. but Aurex finished.\n");
				destory();
				frame->endFrame();
				return 0;
			}
			break;
		}

		if(frame->checkTime() == 0)
			continue;

//		cycle=CYCLE_FOR_64HZ;
#ifdef CYCLE_IS_SLICED
		int c;
#endif
		cycle = CYCLE_FOR_64HZ * option->get_cpu_speed() / 100;
		bool cont = false;
		list<Threadable *>::iterator thread;

		thread=threadList.begin();

		while(thread != threadList.end())
		{
			try {
#ifdef CYCLE_IS_SLICED
				c = cycle/threadList.size();
				cont = (*thread)->execute(&c);
#else
				cont = (*thread)->execute(&cycle);
#endif
			}
			catch(UndefinedInstructionException &e) {

				Log::getInstance()->output(Log::Error, "\nError. undefined instruction. %s\n", e.getRegisterString() );
				cont = printErrorMessageForUndefinedInstruction(&e);
				if( cont == false )
				{
					closeAllThread();
					break;
				}
			}
			catch(OddAccessException &e) {

				Log::getInstance()->output(Log::Error, "\nError. multi-byte mov instruction to/from odd address. %s\n", e.getRegisterString() );
				cont = printErrorMessageForOddAccess(&e);
				if( cont == false )
				{
					closeAllThread();
					break;
				}
			}

			if(cont == false)
			{
#if 0
				if((*thread)->getState()->getState() == THREAD_EXIT)
				{
			// dos_exit のため 全てのスレッドを削除する
					closeAllThread();
					break;
				}
				else
#endif


				{
					if( (*thread) == mainThread )
					{
#ifdef DEBUG
						fprintf(stderr, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		fflush(stderr);
#endif
						mainThread = NULL;
					}

					if( (*thread) == childThread )
					{
						childExitCode = childThread->getExitCode();
						childThread = NULL;

				// 元に戻す
						this->psp_address = this->psp_address_save;
						mainThreadState.setState( ThreadState::RUNNING );
					}

					delete *thread;
					thread=threadList.erase(thread);
				}

				if( !mainThreadState.isRunning() )
					break;

			}
			else
				thread++;
		}

// 実行後メインスレッドが止まっていたら以下は実行させない
		if( !mainThreadState.isRunning() )
			continue;

		if( ruputerState.isEnding() )
			ruputerState.incrementEndingCount();


// 64Hz 割り込み
		interrupt64Hz();

		{
			int no;

			no=frame->getFrameNo();
			switch(no)
			{
		// LCDの64Hz同期信号
			case 1:
				systemAddress->set_lcdSyncSignal();
				break;
			case 2:
				systemAddress->clear_lcdSyncSignal();
				break;

			case 8:
				interrupt1Hz();
				break;
			case 0:
			case 16:
			case 32:
			case 48:
				interrupt4Hz();
				break;
			}
		}

#ifdef NO_SKIP
		frame->isFrameUpdateSkip();
#else
		if(!frame->isFrameUpdateSkip())
#endif

		{
			updateFrame();
			frame->endFrameUpdate();
		}

#if 0
#ifdef DEBUG
		else
		{
			fprintf(stderr, "skip\n");
			fflush(stderr);
		}
#endif
#endif

	}
	return 0;
}


/** 64Hz タイマの割り込み処理
 */
int Ruputer::interrupt64Hz(void)
{
	timermgr->decrement64HzTimer();
	systimermgr->decrement64HzTimer();
	return 0;
}

/** 4Hz タイマの割り込み処理
 */
int Ruputer::interrupt4Hz(void)
{
	systimer->decrement();
	timermgr->decrement4HzTimer();
	systimermgr->decrement4HzTimer();
	return 0;
}

/** 1Hz タイマの割り込み処理
 */
int Ruputer::interrupt1Hz(void)
{
	timermgr->decrement1HzTimer();
	systimermgr->decrement1HzTimer();

#ifdef DEBUG
	fprintf(stderr, "1Hz timer is up\n");
	fflush(stderr);
#endif
//
	if(systemAddress->get_eventrequest() & EVENT_SEC)
	{
#ifdef DEBUG
		fprintf(stderr, "1Hz timer Event is up\n");
		fflush(stderr);
#endif
// というよりも、一番前のものに、 0x8000 をやる感じかもしれない。
		systemAddress->bset_event(EVENT_SEC);
		keybuf->bsetFirstItem(0x8000);
	}


	watchdt->updateWatchDT();
	return 0;
}

/** 画面の更新。
 * 64Hz で呼ばれるが、実行フレーム保持のため、画面更新はスキップされることもある
 */
void Ruputer::updateFrame(void)
{
	cmlcd->updatePixel();

/* 実際の画面への描画 */
	sdlcd->updateWindow();
}

RupAddress *Ruputer::getSystemAddress(void)
{
	return systemAddress;
}

/** sdlcd のセッター
 */
void Ruputer::setSDLcd(SDLcd *sdlcd)
{
	this->sdlcd = sdlcd;
}
SDLcd *Ruputer::getSDLcd(void)
{
	return sdlcd;
}
CMLcd *Ruputer::getCMLcd(void)
{
	return cmlcd;
}


SDSound *Ruputer::getSDSound(void)
{
	return sdsound;
}

void Ruputer::setSDSound(SDSound *sdsound)
{
	this->sdsound = sdsound;
}



RuputerDirectory *Ruputer::getRuputerDirectory(void)
{
	return ruputerDirectory;
}

Directory *Ruputer::getLocalDirectory(void)
{
	return localDirectory;
}

list<Threadable *> *Ruputer::getThreadList(void)
{
	return &threadList;
}
KeyBuffer *Ruputer::getKeyBuffer(void)
{
	return keybuf;
}

MemoryForCpu *Ruputer::getMemoryForCpu(void)
{
	return memoryForCpu;
}

unsigned long Ruputer::getPspAddress(void)
{
	return psp_address;
}

void Ruputer::setPspAddress(unsigned long psp)
{
	this->psp_address = psp;
}

unsigned long Ruputer::getDtaAddress(void)
{
	return dta_address;
}
void Ruputer::setDtaAddress(unsigned long dta)
{
	this->dta_address = dta;
}

RuputerState *Ruputer::getState(void)
{
	return &ruputerState;
}

TimerMgr *Ruputer::getTimerMgr(void)
{
	return timermgr;
}

TimerMgr *Ruputer::getSysTimerMgr(void)
{
	return systimermgr;
}

WatchDT *Ruputer::getWatchDT(void)
{
	return watchdt;
}


/** load する
 * @param filename 相対パスでも可？
 * む。もうちょい考える。
 * @exception ExfFileNotFoundException, ExfFileInvalidException
 */
int Ruputer::load(char const *filename) throw(ExfFileInvalidException, ExfFileNotFoundException, AssociatedFileNotInVirtualDirectoryException)
{
	delete exffile;
	exffile = NULL;

	unsigned long psp_addr;
	char *psp;
	char exf[256];
	int exf_flag;

	exf_flag = getExecutableFile(filename, exf);

#ifdef DEBUG
	fprintf(stderr, "Ruptuer::load exf (%s)\n", exf);
#endif

	exffile = new ExfFile( exf );
	exffile->loadToMemory();
	psp_addr = exffile->getPsp();

	psp = (char *)Memory::getMemoryPointer(psp_addr);

// Ruputer 側のカレントディレクトリの設定
// exf ではなく filename で行なうことに注意
	try {
		DirectoryConverter torup(localDirectory, ruputerDirectory);
		char fullpath[1024], ruputerpath[256];

		if( localDirectory->isAbsoluteDirectory(filename) )
			strcpy(fullpath, filename);
		else
		{
	// load ファイルが相対パスだった → 絶対パスに変換する。
	// 理由：絶対パスに変換し、その後仮想ルートディレクトリより下かどうか、の判定を行なう。
			getcwd(fullpath, MAX_PATH);
			char *p;
			p = strchr(fullpath, 0);
			*p = localDirectory->getPathDelimiter();
			strcpy(p+1, filename);
		}

		torup.convert(fullpath, ruputerpath);
// assocext ではなく filename を変換したものを格納する、こと。
		strcpy(psp + A_PSP_ARG, ruputerpath); // 0x100 - 0x88 = 0x78 = 120 byte に制限した方がいいかも。

		{
			char dir[1024], file[256];
	//			localDirectory->splitDirFile(ruputerpath, dir, file);
			ruputerDirectory->splitDirFile(ruputerpath, dir, file);
#ifdef DEBUG
			fprintf(stderr, "!! Ruputer::load ruputerpath: %s ====> dir: %s, file: %s\n", ruputerpath, dir, file);
			fflush(stderr);
#endif
			ruputerDirectory->chdir(dir);
		}
	}
	catch( DirectoryConverterException &e )
	{
// load するファイルが *.exf でなく、
// PSP 領域に書き込むときにこの例外を拾うということは実行不可能。（逆変換に失敗→仮想ルートディレクトリ以外）
		if( exf_flag != 1 )
		{
			Log::getInstance()->output(Log::Error, "\n%s path convertion error. please check %s file is in virtual root directory.\n", filename, filename);
			throw AssociatedFileNotInVirtualDirectoryException();
		}

		Log::getInstance()->output(Log::Warning, "\n%s path convertion warning. please check %s file is in virtual root directory. This emulation program is not affected by this error, and a emulation continue.\n\
aurex doesn't set PSP + 0x80 (args buffer) and  current directory is 'b:\' by force.\n", filename, filename);

// 変換失敗→ b:\ がエミュレーションのルートディレクトリ
		ruputerDirectory->chdir("b:\\");

	}

	memcpy(psp + A_PSP_SYS, A_SYSTEMCTL_STR, 8);
	this->psp_address = psp_addr;

#define BOOT_STACK 0x120000
	mainThread = new MainThread(exffile->getPc(), BOOT_STACK, this, A_KERNEL_MAIN_SP, &mainThreadState);
	mainThreadState.setState( ThreadState::RUNNING);
	ruputerState.setState( RuputerState::RUNNING);

	threadList.push_front( mainThread );

	{
		char tmp[1024];
		char const *vdir;
		ruputerDirectory->getdir(tmp);
		vdir = option->get_root_directory();
		if( *vdir ==  '\0' )
			vdir = "(not define)";

		Log::getInstance()->output(Log::Info,
								   "\
load file (local)       : %s\n\
assoc exf file (local)  : %s\n\
virtual root dir        : %s\n\
ruputer current dir     : %s\n",
				filename,
				exf,
				vdir,
				tmp);
	}

	return 0;
}

void Ruputer::loadChild( char const *filename, char *newargs )
{
	Log::getInstance()->output(Log::Debug, "load child process filename:%s\n", filename );
	Log::getInstance()->output(Log::Debug, "load child process args:%s\n", newargs );

	unsigned long psp_addr;
	char *psp;
	ExfFile *childexf;

	childexf = new ExfFile( filename );
	childexf->loadToMemory();
	psp_addr = childexf->getPsp();

	psp = (char *)Memory::getMemoryPointer( psp_addr );

	strcpy( psp + A_PSP_ARG, newargs );
	memcpy( psp + A_PSP_SYS, A_SYSTEMCTL_STR, 8 );

// 退避しておく
	this->psp_address_save = this->psp_address;
	this->psp_address = psp_addr;

	childThread = new ChildThread( childexf->getPc(), mainThread->getStackPointer(), this, A_KERNEL_MAIN_SP );
	mainThreadState.setState( ThreadState::CHILD_RUNNING );
	childThread->getState()->setState( ThreadState::RUNNING );
//	childThreadState.setState( ThreadState::RUNNING );

	threadList.push_front( childThread );

	delete childexf;
}

int Ruputer::getChildExitCode(void)
{
	return childExitCode;
}


/** 初期化
 * 最初には呼び出す必要は無い。
 * ロード仕直すという場合はロード前に呼び出すとよい。
 */
void Ruputer::init(void)
{
	closeAllThread();
	timermgr->closeAll();
	cmlcd->blinkmgr->closeAll();
	cmlcd->reversemgr->closeAll();

	ruputerState.setState( RuputerState::STOP );
	mainThreadState.setState( ThreadState::EXIT );
	keybuf->flush();
	frame->startFrame();

// システム使用のメモリのクリア
	systemAddress->set_eventregister(0);
	systemAddress->set_eventregister(0);
	systemAddress->set_event(0);
	systemAddress->set_timerstat(0);
	systemAddress->set_keyport(0);
	systemAddress->set_lcdPlace(0, 0);
}

/** 読み直し
 * 現在実行中／実行終了したものを初めから実行する。
 * ただしファイルからの読み直しは行なわないため、コード領域が書き変わる物については動作しない
 * 可能性がある。
 */
void Ruputer::reload(void)
{
	init();

	if( exffile )
	{
		mainThread = new MainThread(exffile->getPc(), BOOT_STACK, this, A_KERNEL_MAIN_SP, &mainThreadState);
		ruputerState.setState( RuputerState::RUNNING );
		mainThreadState.setState( ThreadState::RUNNING );

		threadList.push_front( mainThread );
	}

}

/** threadを全て終了する
 */
void Ruputer::closeAllThread(void)
{
	list<Threadable *>::iterator thread;

	thread=threadList.begin();

	while(thread != threadList.end())
	{
		delete *thread;
		thread=threadList.erase(thread);
	}
}

#if 0
/** オプションを読み込む
 * @param dir=NULL オプション(option.txt)があるディレクトリ。NULL でカレントディレクトリに存在するとする
 * @retval 0 成功
 * @retval 1 失敗
 */
int Ruputer::loadOption(Directory *dir)
{
#if 0
	optionmgr->load("option.txt");
	optionmgr->callCallbackFunction();
#endif
	return 0;
}

/** /system/filer.ini, ユーザ指定の filer.ini を読み込む
 * @param dir=NULL aurex.ini があるディレクトリ。NULL でカレントディレクトリ相対
 * @retval 0 成功
 * @retval 1 失敗
 */
int Ruputer::loadFilerIni(Directory *dir)
{
	filerini->loadUFilerIni(dir, "aurex.ini");
	return 0;
}
#endif

/**
 * @param local のfilename *.exf の他にも、*.txt とかでありうる。
 * @param exf filename が *.exf だったら、filename と同じ。aurex.ini に一致る場合はそれに結びついている実行ファイルをローカルのパスに変換したもの。
 * @exception ExfFileNotFoundException
 * @retval 0 filename は exf ファイルだった （todo 専用のメソッド追加した方がいい？）
 * @retval 1 filename は 上記以外だ
 *
 */
int Ruputer::getExecutableFile(char const *filename, char *localExf) throw(ExfFileNotFoundException)
{
	assert( localExf );
	assert( filename );

// filename が *.exf かの判定
	char *p;
	p = strrchr(filename, '.');
	if( p != NULL
//		&& stricmp(p, ".exf") == 0 )
#ifdef unix
		&& strcasecmp(p, ".exf") == 0
#else
		&& stricmp(p, ".exf") == 0
#endif
		)
	{
		strcpy(localExf, filename);
		return 1;
	}

	char const *assocexf;

	assocexf = filerini->getExecutableFileName(filename);
	if(assocexf != NULL)
	{
		try {
			DirectoryConverter tohost(ruputerDirectory, localDirectory);
			tohost.convert(assocexf, localExf);
		}
		catch( DirectoryConverterException &e ) {
	// コンバート失敗

	// 開くファイルが無い
			Log::getInstance()->output(Log::Error, "\n%s path convertion error. please check option virtual root directory.\n", assocexf);

			throw ExfFileNotFoundException();
		}
	}
	else
	{
// exf じゃないのに実行させるのは良くない？
//		strcpy(localExf, filename);

		Log::getInstance()->output(Log::Error, "\n%s is not associated in aurex.ini.\n", filename);

		throw ExfFileNotFoundException();
	}

	Log::getInstance()->output(Log::Info, "\nassoc exf file (Ruputer): %s\n", assocexf);

	return 0;
}

/**
 * ログの設定継承する。
 */
void Ruputer::setupLog(void)
{
	switch( ::option->get_log_type() )
	{
	case Log::TYPE_NULL:
		Log::getInstance()->setLogOutput( NULL );
		break;
	case Log::TYPE_STDERR:
		Log::getInstance()->setLogOutput( new LogOutputStderr() );
		break;
	case Log::TYPE_FILE:
		Log::getInstance()->setLogOutput( new LogOutputFile( ::option->get_log_file() ) );
		break;
	case Log::TYPE_CURSES:
	case Log::TYPE_WINDOWS_EVENT:
		break; // define subclass
	}

	Log::getInstance()->setLogLevel( (enum Log::LogLevel) ::option->get_log_level() );
}

/**
 * 継承する。
 * デフォルトだと、無条件に続ける。
 */
bool Ruputer::printErrorMessageForUndefinedInstruction(UndefinedInstructionException *e)
{
	return true;
}

/**
 *
 */
bool Ruputer::printErrorMessageForOddAccess(OddAccessException *e)
{
	return true;
}


void Ruputer::deleteMainThread(void)
{
	threadList.remove( mainThread );
	delete mainThread;
	mainThread = NULL;
}

void Ruputer::requestDestroy(void)
{
	ruputerState.setState( RuputerState::ENDING_DESTROY );
	ruputerState.startEndingCount();
}

void Ruputer::destory(void)
{
}

void Ruputer::requestReload(void)
{
	ruputerState.setState( RuputerState::ENDING_RELOAD );
	ruputerState.startEndingCount();
}

void Ruputer::requestNewLoad(char const *fileName)
{
	delete newLoadFileName;

	int len = strlen( fileName );

	newLoadFileName = new char [ len + 1];
	memcpy(newLoadFileName, fileName, len + 1);

	ruputerState.setState( RuputerState::ENDING_NEW_LOAD );
	ruputerState.startEndingCount();
}

