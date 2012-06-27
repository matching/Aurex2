/*
 * psdos.cpp - psdos function emulation
 *
 * $Id: psdos.cpp,v 1.26 2008/07/26 06:43:08 fumi Exp $
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
 * Ps-Dos interrupt routine
 *
 * 呼び出しの性質上、24 bit 化する必要ない
 * ただし、memory に足して云々するものは(to_linear つかっているもの)、
 * やる必要がある。
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include <sys/stat.h>

#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "cpu.h"
#include "state.h"
#include "psdos.h"
#include "x86reg.h"
#include "thread.h"
#include "memory.h"
#include "ruputer.h"
#include "ruputerdirectory.h"
#include "directoryconverter.h"

#include "log.h"

DosIntr::DosIntr(Ruputer *ruputer, Thread *thread)
{
	this->thread = thread;
	this->threadState = thread->getState();
	this->ruputer = ruputer;
	this->rupdir = ruputer->getRuputerDirectory();
	this->localdir = ruputer->getLocalDirectory();

	assert( thread );
	assert( threadState );
	assert( rupdir );
	assert( localdir );
	assert( threadState );
}


void DosIntr::callDosIntr(Register *regs)
{
	X86Reg *x86reg=new X86Reg(regs->a[0]);
	int no;

	no=x86reg->read_AH();

	Log::getInstance()->output(Log::Debug, "PSDos interrupt no.0x%lx\n", no);

	switch(no)
	{
	case 0x4b:
		dos_execute(x86reg);
		break;
	case 0x4d:
		dos_child_ret(x86reg);
		break;
	case 0x2a:
		dos_getdate(x86reg);
		break;
	case 0x2c:
		dos_gettime(x86reg);
		break;
#if 1
	case 0x5b:
		dos_crnew(x86reg);
		break;
	case 0x3c:
		dos_create(x86reg);
		break;
	case 0x3d:
		dos_open(x86reg);
		break;
	case 0x3e:
		dos_close(x86reg);
		break;
	case 0x3f:
		dos_read(x86reg);
		break;
	case 0x40:
		dos_write(x86reg);
		break;
	case 0x41:
		dos_delete(x86reg);
		break;
	case 0x42:
		dos_seek(x86reg);
		break;
	case 0x36:
		dos_dfspace(x86reg);
		break;
	case 0x56:
		dos_rename(x86reg);
		break;
	case 0x5a:
		dos_crtmp(x86reg);
		break;
#endif

	case 0x4e:
		dos_find_first(x86reg);
		break;
	case 0x4f:
		dos_find_next(x86reg);
		break;


	case 0x3b:
		dos_chdir(x86reg);
		break;
	case 0x47:
		dos_getcdir(x86reg);
		break;
	case 0x39:
		dos_mkdir(x86reg);
		break;
	case 0x3a:
		dos_rmdir(x86reg);
		break;
	case 0x0e:
		dos_seldrv(x86reg);
		break;
	case 0x1b:
		dos_getcdrive(x86reg);
		break;

	case 0x1c:
		dos_gettdrive(x86reg);
		break;
	case 0x19:
		dos_getcdrvno(x86reg);
		break;

	case 0x2f:
		dos_getdta(x86reg);
		break;
	case 0x1a:
		dos_setdta(x86reg);
		break;

	case 0x4c:
		dos_exit(x86reg, regs);
		break;
	case 0x62:
		dos_get_psp(x86reg);
		break;

	default:
		Log::getInstance()->output(Log::Warning, "Aurex doesn't support this PSDos intterupt no.0x%lx\n", no);
		break;
	}
#ifdef DEBUG
	fprintf(stderr, " dos int end");
#endif

	delete x86reg;
}


void DosIntr::dos_getdate(X86Reg *x86reg)
{
	time_t t;
	struct tm *local;

	t=time(NULL);
	local=localtime(&t);

	x86reg->write_CX(local->tm_year+1900);
	x86reg->write_DH(local->tm_mon+1);
	x86reg->write_DL(local->tm_mday);
	x86reg->write_AL(local->tm_wday);
}

void DosIntr::dos_gettime(X86Reg *x86reg)
{
	time_t t;
	struct tm *local;

	t=time(NULL);
	local=localtime(&t);

	x86reg->write_CH(local->tm_hour);
	x86reg->write_CL(local->tm_min);
	x86reg->write_DH(local->tm_sec);

/* ドキュメント上は、1/100 秒を代入するとあるが、実機では 0
 */
	x86reg->write_DL(0);
}


void DosIntr::dos_seldrv(X86Reg *x86reg)
{
//	p86reg->h.al=1;
	x86reg->write_AL(1);
}

void DosIntr::dos_getcdrive(X86Reg *x86reg)
{
//	p86reg->h.al=0;

	x86reg->write_AL(0);
	dos_dfspace(x86reg);
}

void DosIntr::dos_gettdrive(X86Reg *x86reg)
{
	dos_dfspace(x86reg);
}

void DosIntr::dos_getcdrvno(X86Reg *x86reg)
{
	x86reg->write_AL(0);
//	p86reg->h.al=1;
}



/*
 * ds : directry segment address
 * si :           offset address
 *
 * cflag : 0 success
 *         1 failur
 */

void DosIntr::dos_getcdir(X86Reg *x86reg)
{
	unsigned long addr;
	char *directory;
	char temp[MAX_PATH];
	int res;

	addr=convToLinearAddress(x86reg->read_DS(), x86reg->read_SI());
	directory=(char *)Memory::getMemoryPointer(addr);

#ifdef DEBUG
	fprintf(stderr, "!! getdir %p %x\n", directory, addr);
	fflush(stderr);
#endif

	res = rupdir->getdir(temp);
// temp は b:\foo\bar とかえってくるが b: は捨てる

	if(res != 0)
	{
		x86reg->write_CFLAG(1);
		return;
	}

	strcpy(directory, temp + 2);

#ifdef DEBUG
	fprintf(stderr, "!! getdir (%s) end \n", directory);
	fflush(stderr);
#endif

	x86reg->write_CFLAG(0);
}

/*
 * ds : directry segment address
 * dx :           offset address
 *
 * cflag : -1 (0xffff) failur
 *       :  0  success
 */
void DosIntr::dos_chdir(X86Reg *x86reg)
{
	unsigned long addr;
	char *directory;
	int res;

//	addr=convToLinearAddress(x86reg->read_DS(), x86reg->read_SI());
	addr=convToLinearAddress(x86reg->read_DS(), x86reg->read_DX());
	directory=(char *)Memory::getMemoryPointer(addr);

	res=rupdir->chdir(directory);
	if(res != 0)
	{
		x86reg->write_CFLAG(1);
		return;
	}
	x86reg->write_CFLAG(0);
}

void DosIntr::dos_mkdir(X86Reg *x86reg)
{
	unsigned long addr;
	char *strdir;
	int res;
	char strlocal[1024];

	addr=convToLinearAddress(x86reg->read_DS(), x86reg->read_SI());
	strdir=(char *)Memory::getMemoryPointer(addr);

	try{
		DirectoryConverter c(rupdir, localdir);
		res = c.convert(strdir, strlocal);
	}
	catch( DirectoryConverterException &e ) {
		x86reg->write_AX(3); //?
		x86reg->write_CFLAG(1);
		return;
	}

	if(res != 0)
	{
		x86reg->write_CFLAG(1);
		return;
	}

//	res=sdfile->createDirectory(directory);
#ifdef unix
	res = mkdir(strlocal, 0755);
#else
	res = mkdir(strlocal);
#endif
	if(res != 0)
	{
		x86reg->write_CFLAG(1);
		return;
	}
	x86reg->write_CFLAG(0);
}

void DosIntr::dos_rmdir(X86Reg *x86reg)
{
	unsigned long addr;
	char *strdir;
	char strlocal[1024];
	int res;

	addr=convToLinearAddress(x86reg->read_DS(), x86reg->read_SI());
	strdir=(char *)Memory::getMemoryPointer(addr);

	try{
		DirectoryConverter c(rupdir, localdir);
		res = c.convert(strdir, strlocal);
	}
	catch( DirectoryConverterException &e ) {
		x86reg->write_CFLAG(1);
		return;
	}

	if(res != 0)
	{
		x86reg->write_CFLAG(1);
		return;
	}
//	res=sdfile->removeDirectory(directory);
	res = rmdir( strlocal );
	if(res != 0)
	{
		x86reg->write_CFLAG(1);
		return;
	}
	x86reg->write_CFLAG(0);
}

/*
 * ds : dta segment address
 * dx :      offset address
 *
 */
void DosIntr::dos_setdta(X86Reg *x86reg)
{
//	::ruputer->dta_address = convToLinearAddress(x86reg->read_DS(), x86reg->read_DX());
	ruputer->setDtaAddress(convToLinearAddress(x86reg->read_DS(), x86reg->read_DX()));
//	dta=to_24bit(to_linear(p86reg->x.ds, p86reg->x.dx));
}

/*
 *
 * es : dta segment address
 * bx :     offset  address
 */
void DosIntr::dos_getdta(X86Reg *x86reg)
{
	unsigned long dta;

	dta = ruputer->getDtaAddress();

	x86reg->write_ES((dta >> 16) & 0xff);
	x86reg->write_BX( dta & 0xffff);
}

/*
 *  ﾌﾗｯｼｭﾒﾓﾘｻｲｽﾞ ﾌﾞﾛｯｸ ｾｸﾀ ｸﾗｽﾀ ﾍｯﾀﾞ
 *     2048KB      512   8  240   2  << ﾃﾞﾌｫﾙﾄ値
 *
 * dl : drive no.
 *
 * エラー ax == 0xffff
 *
 * dx : clusster / drive
 * ax : secter / clusster
 * cx : byte / sector
 * bx : available clusster
 *
 */
void DosIntr::dos_dfspace(X86Reg *x86reg)
{
	int drive;

	drive=x86reg->read_DL();

#ifdef DEBUG
	fprintf(stderr, "dos_dfspace %d\n", drive);
	fflush(stderr);
#endif

	if(drive != 0 && drive != 2)	/* B ドライブ以外 */
	{
		x86reg->write_AX(0xffff);
		return;
	}

#define D_HEADER 2
#define D_CLUSSTER 240
#define D_SECTOR_CLUSSTER 8
#define D_BYTE_SECTOR 512
	x86reg->write_DX(D_CLUSSTER*D_HEADER);
	x86reg->write_AX(D_SECTOR_CLUSSTER);
	x86reg->write_CX(D_BYTE_SECTOR);

	x86reg->write_BX(D_CLUSSTER*D_HEADER); /* すべて使用可 */
}

void DosIntr::dos_exit(X86Reg *x86reg, Register *regs)
{
	regs->d[0]=x86reg->read_AL(); /* return value */
//	DosIntr::flag=DOSINTR::FINISH;
	threadState->setState( ThreadState::EXIT );

	if( ! thread->isMainThread() )
		ruputer->deleteMainThread();
}


void DosIntr::dos_get_psp(X86Reg *x86reg)
{
	unsigned long psp;

	psp = ruputer->getPspAddress();

	x86reg->write_ES((psp >> 16) & 0xff);
	x86reg->write_BX( psp & 0xffff);
}




