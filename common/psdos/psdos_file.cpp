/*
 * psdos_file.cpp - psdos function emulation for file i/o
 *
 * $Id: psdos_file.cpp,v 1.17 2005/01/13 15:13:47 fumi Exp $
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
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "x86reg.h"
#include "psdos.h"
#include "fdmgr.h"
#include "memory.h"

#include "directoryconverter.h"
#include "ruputerdirectory.h"
#include "ruputer.h"
#include "thread.h"

/***********************************************************************/
/* ds : filename segment address
 * dx :                  offset
 * cx : file attribute
 *
 * cflag & 0x01 == 0 で成功
 * ax : ファイルハンドル
 *
 *   存在→0バイトにして開く
 * 非存在→作る
 *
 */
void DosIntr::dos_create(X86Reg *x86reg)
{
	unsigned long addr;
	char const *filename;
	char full_path[MAX_PATH];
	int res;

	addr=convToLinearAddress(x86reg->read_DS(), x86reg->read_DX());
	filename=(char const *)Memory::getMemoryPointer(addr);

#ifdef DEBUG
	fprintf(stderr, "!! DosIntr::dos_create filename %s\n", filename);
	fflush(stderr);
#endif

	try{
		DirectoryConverter c(rupdir, localdir);
		res = c.convert(filename, full_path);
	}
	catch( DirectoryConverterException &e ) {
#ifdef DEBUG
		fprintf(stderr, "!! DosIntr::dos_create catch DirectoryConverterException\n");
		fflush(stderr);
#endif
		x86reg->write_AX(3); //?
		x86reg->write_CFLAG(1);
		return;
	}

	if(res != 0)
	{
		x86reg->write_AX(3); //?
		x86reg->write_CFLAG(1);
		return;
	}

	FILE *fp;
	fp=fopen(full_path, "w+b");
	if(fp == NULL)
	{
		x86reg->write_AX(3);
		x86reg->write_CFLAG(1);
		return;
	}

	res=FDMgr::getInstance()->addFD(fp);

	x86reg->write_AX(res);
	x86reg->write_CFLAG(0);
}


/**************************************************************************
 * ds : filename segment address
 * dx :                  offset
 * cx : file attribute
 *
 *   存在→エラー
 * 非存在→作る
 *
 */
void DosIntr::dos_crnew(X86Reg *x86reg)
{
	int res;
	char const *filename;
	char full_path[MAX_PATH];
	unsigned long addr;

	addr=convToLinearAddress(x86reg->read_DS(), x86reg->read_DX());
	filename=(char const *)Memory::getMemoryPointer(addr);

#ifdef DEBUG
	fprintf(stderr, "!! DosIntr::dos_crnew filename %s\n", filename);
	fflush(stderr);
#endif
	try {
		DirectoryConverter c(rupdir, localdir);
		res = c.convert(filename, full_path);
	}
	catch( DirectoryConverterException &e ) {
#ifdef DEBUG
		fprintf(stderr, "!! DosIntr::dos_crnew catch DirectoryConverterException\n");
		fflush(stderr);
#endif
		x86reg->write_AX(3);
		x86reg->write_CFLAG(1);
		return;
	}

	if(res != 0)
	{
		x86reg->write_AX(3);
		x86reg->write_CFLAG(1);
		return;
	}

	if(isLocalFileExist(full_path))
	{
// 同名のファイルが存在
		x86reg->write_AX(80);
		x86reg->write_CFLAG(1);
		return;
	}

	FILE *fp;
	fp=fopen(full_path, "w+b");
	if(fp == NULL)
	{
		x86reg->write_AX(3);
		x86reg->write_CFLAG(1);
		return;
	}

	res=FDMgr::getInstance()->addFD(fp);
	x86reg->write_AX(res);
	x86reg->write_CFLAG(0);
}



/* ds : filename segment address
 * dx :                  offset
 * al : open mode 0:read, 1:write, 2:read/write
 *
 * cflag & 0x01 == 0 で成功
 * ax : ファイルハンドル
 *
 * 存在しないとエラー？
 */

void DosIntr::dos_open(X86Reg *x86reg)
{
	int res;
	char const *filename;
	char full_path[MAX_PATH];
	unsigned long addr;

	addr=convToLinearAddress(x86reg->read_DS(), x86reg->read_DX());

#ifdef DEBUG
	fprintf(stderr, "!! DosIntr::dos_open address %x\n", addr);
	fflush(stderr);
#endif

	filename=(char const *)Memory::getMemoryPointer(addr);

#ifdef DEBUG
	fprintf(stderr, "!! DosIntr::dos_open filename %s mode %d\n", filename, x86reg->read_AL());
	fflush(stderr);
#endif
	try {
		DirectoryConverter c(rupdir, localdir);
		res = c.convert(filename, full_path);
	}
	catch( DirectoryConverterException &e ) {
#ifdef DEBUG
		fprintf(stderr, "!! DosIntr::dos_open catch DirectoryConverterException\n");
		fflush(stderr);
#endif
		x86reg->write_AX(2);
		x86reg->write_CFLAG(1);
		return;
	}

#if 1
	if(!isLocalFileExist(full_path))
	{
#ifdef DEBUG
		fprintf(stderr, "!! DosIntr::dos_open error file not found %s\n", full_path);
		fflush(stderr);
#endif
		x86reg->write_AX(2);
		x86reg->write_CFLAG(1);
		return;
	}
#endif

	FILE *fp;
	static char *open_mode[]={"rb", "wb", "wb+"};
	fp=fopen(full_path, open_mode[x86reg->read_AL()]);
	if(fp == NULL)
	{
		x86reg->write_AX(80);
		x86reg->write_CFLAG(1);
		return;
	}

	res=FDMgr::getInstance()->addFD(fp);

#ifdef DEBUG
	fprintf(stderr, "!! DosIntr::dos_open pair %d %p %s\n", res, fp, full_path);
	fflush(stderr);
#endif


	x86reg->write_AX(res);
	x86reg->write_CFLAG(0);
}
/*
 * bx: file handle
 *
 * cflag=0 success
 */
void DosIntr::dos_close(X86Reg *x86reg)
{
	FILE *fp;

	fp=FDMgr::getInstance()->getFP(x86reg->read_BX());

#ifdef DEBUG
	fprintf(stderr, "!! DosIntr::dos_close pair %d %p\n", x86reg->read_BX(), fp);
	fflush(stderr);
#endif
	FDMgr::getInstance()->delFD(x86reg->read_BX());
	if( fp == NULL )
	{
		x86reg->write_CFLAG(1);
		return;
	}
	fclose(fp);

	x86reg->write_CFLAG(0);
}
/* ds : buffer  segment address
 * dx :                  offset
 * bx : file handle
 * cx : bytes
 *
 * cflag & 0x01 == 0 で成功
 * ax : 読み込んだバイト数
 */
void DosIntr::dos_read(X86Reg *x86reg)
{
	char *buf;
	int n;
	unsigned long addr;
	FILE *fp;

	fp=FDMgr::getInstance()->getFP(x86reg->read_BX());

#ifdef DEBUG
	fprintf(stderr, "!! DosIntr::dos_read pair %d %p\n", x86reg->read_BX(), fp);
	fflush(stderr);
#endif
	if( fp == NULL )
	{
		x86reg->write_CFLAG(1);
		return;
	}


	addr=convToLinearAddress(x86reg->read_DS(), x86reg->read_DX());
	buf=(char *)Memory::getMemoryPointer(addr);

	n=fread(buf, 1, x86reg->read_CX(), fp);

	if(n > 0)
		x86reg->write_CFLAG(0);
	else
		x86reg->write_CFLAG(1);

	x86reg->write_AX(n);
}
/* ds : buffer  segment address
 * dx :                  offset
 * bx : file handle
 * cx : bytes
 *
 * cflag & 0x01 == 0 で成功
 * ax : 読み込んだバイト数
 */
void DosIntr::dos_write(X86Reg *x86reg)
{
	char const *buf;
	int n;
	FILE *fp;
	unsigned long addr;

#ifdef DEBUG
	fprintf(stderr, "!! DosIntr::dos_write fds %d\n", x86reg->read_BX());
	fflush(stderr);
#endif

	fp=FDMgr::getInstance()->getFP(x86reg->read_BX());

#ifdef DEBUG
	fprintf(stderr, "!! DosIntr::dos_write pair %d %p\n", x86reg->read_BX(), fp);
	fflush(stderr);
#endif
	if( fp == NULL )
	{
		x86reg->write_CFLAG(1);
		return;
	}

	addr=convToLinearAddress(x86reg->read_DS(), x86reg->read_DX());
	buf=(char const *)Memory::getMemoryPointer(addr);


#ifdef DEBUG
	fprintf(stderr, "!! DosIntr::dos_write FILE %p, buf address %p, write size %d\n", fp,  buf, x86reg->read_CX());
	fflush(stderr);
#endif

	n=fwrite(buf, 1, x86reg->read_CX(), fp);

	if(n > 0)
		x86reg->write_CFLAG(0);
	else
		x86reg->write_CFLAG(1);

	x86reg->write_AX(n);
}
/*
 * al : mode
 *       0=ファイル先頭からのバイトオフセット
 *       1=ファイルポインタの現在位置からのバイトオフセット
 *       2=ファイルの終わりからのバイトオフセット
 * bx : file handle
 * cx : offset 上位 16 bit
 * dx :        下位 16 bit
 *
 * cflag & 0x01 == 0 で成功
 * dx :新ファイルポインタ上位 16 bit
 * ax :                  下位 16 bit
 */
void DosIntr::dos_seek(X86Reg *x86reg)
{
	static int whence[3]={SEEK_SET, SEEK_CUR, SEEK_END};
	long offset;
	FILE *fp;
	int ret;

	fp=FDMgr::getInstance()->getFP(x86reg->read_BX());

#ifdef DEBUG
	fprintf(stderr, "!! DosIntr::dos_seek pair %d %p\n", x86reg->read_BX(), fp);
	fflush(stderr);
#endif
	if( fp == NULL )
	{
		x86reg->write_CFLAG(1);
		return;
	}

	offset=x86reg->read_CX() << 16;
	offset|=x86reg->read_DX();
	ret=fseek(fp, offset, whence[x86reg->read_AL()]);
	if(ret != 0)
	{
// failur
		x86reg->write_CFLAG(1);
		return;
	}

	{
		unsigned long newoffset;

		newoffset=ftell(fp);

		x86reg->write_DX((newoffset >> 16) & 0xffff);
		x86reg->write_AX(newoffset & 0xffff);
		x86reg->write_CFLAG(0);
	}
}

/* ds : filename segment address
 * dx :                  offset
 * cx : file attribute
 *
 * cflag & 0x01 == 0 で成功
 * ax : ファイルハンドル
 */
void DosIntr::dos_crtmp(X86Reg *x86reg)
{
#if 0
	HANDLE fh;
	int n;
	char *filename;
	char local_filename[256];
	char tmp_filename[256];

	filename=(char *)(to_24bit(to_linear(p86reg->x.ds, p86reg->x.dx))+memory);
	to_local(filename, local_filename);

	GetTempFileName(local_filename, "tmp", 0, tmp_filename);

	fh=CreateFile(tmp_filename, GENERIC_READ|GENERIC_WRITE,
					0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(fh == INVALID_HANDLE_VALUE)
	{
		p86reg->x.ax=3;
		p86reg->x.cflag=1;
		return;
	}

	n=set_file_handle(fh);
	p86reg->x.cflag=0;
	p86reg->x.ax=n;
#endif
}

/* ds : filename segment address
 * dx :                  offset
 *
 * 成功 cflag & 0x01 == 0
 * ax : エラーコード（エラー時）
 */
void DosIntr::dos_delete(X86Reg *x86reg)
{
#if 0
	char *filename;
	char local[256];

	filename=(char *)(to_24bit(to_linear(p86reg->x.ds, p86reg->x.dx))+memory);
	to_local(filename, local);

	p86reg->x.cflag=!DeleteFile(local);
#endif
}
/*
 *	ds = old path segment
 *	dx =          offset
 *	es = new path segment
 *	di =          offset
 */
void DosIntr::dos_rename(X86Reg *x86reg)
{
#if 0
	char *new_filename;
	char *old_filename;
	char new_local[256];
	char old_local[256];

	new_filename=(char *)(to_24bit(to_linear(p86reg->x.ds, p86reg->x.dx))+memory);
	to_local(new_filename, new_local);

	old_filename=(char *)(to_24bit(to_linear(p86reg->x.es, p86reg->x.di))+memory);
	to_local(old_filename, old_local);

	p86reg->x.cflag=!MoveFile(old_filename, new_filename);
#endif
}


int DosIntr::isLocalFileExist(char const *filename)
{
	struct stat st;
	if(stat(filename, &st) == -1)
		return 0;

#ifdef MSVC
	if( st.st_mode & _S_IFREG )
		return 1;
#else
	if(S_ISREG(st.st_mode))
		return 1;
#endif

	return 0;
}
