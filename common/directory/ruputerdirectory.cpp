/*
 * ruputerdirectory.cpp - Ruputer directory module
 *
 * $Id: ruputerdirectory.cpp,v 1.16 2005/01/14 04:09:40 fumi Exp $
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
#include <assert.h>
#include <sys/stat.h>

#include "dirstacksjis.h"

#include "ruputerdirectory.h"
#include "directoryconverter.h"
#include "codingsystem.h"
#include "ruputer.h"

/**
 * Ruputer タイプのディレクトリ
 * DosDirectory を継承している
 *
 * @param directory Directory クラスを継承したインスタンス（システム依存）
 *
 */
RuputerDirectory::RuputerDirectory(Directory *localDirectory)
{
	this->localDirectory = localDirectory;
	drive='b';

	coding = SHIFT_JIS;
}

RuputerDirectory::RuputerDirectory(Directory *localDirectory, char const *path) : DosDirectory(path)
{
	this->localDirectory = localDirectory;
	drive='b';

	coding = SHIFT_JIS;
}
/** Ruputer の残りpath
 * @param src "b:\system\ini\..."（絶対指定） や、"foo"（相対指定） 等
 * @param restpath 後述参照
 * @retval 0 正常
 * @retval 1 異常
 *
 * 処理概要
 *  src が絶対パスで来た場合は、restpath には b:\ が取り除かれる
 *        相対パスで来た場合は、restpath には (Ruputer カレントディレクトリ) + src が格納される
 *
 */
int RuputerDirectory::getRestPath(char const *src, char *restpath) throw(DirectoryException)
{
	DirStack *rest;

	if(*src == '\\')
	{
// 絶対パス
		rest=new DirStackSjis(src+1, '\\');
	}
	else if(*(src+1) == ':' && *(src+2) == '\\')
	{
// 絶対パス（ドライブ付き）
		rest=new DirStackSjis(src+3, '\\');
	}
	else if(*(src+1) == ':' && *(src+2) == 0)
	{
// 絶対パス（ドライブのみ）
		*restpath=0;
		return 0;
	}
	else
	{
// 相対パス（その他）
#if 1
		char str[1024];

		getdir(str);
		strcat(str, "\\");
		strcat(str, src);

#ifdef DEBUG
		fprintf(stderr, "!! RuputerDirectory::rest relative path str = (%s)\n", str);
		fflush(stderr);
#endif

		rest=new DirStackSjis(str+3, '\\');
#else
		rest=new DirStackSjis(src, '\\');
#endif

	}

	rest->normalize();
	rest->toFullPath(restpath, '\\');

	delete rest;

	return 0;
}


int RuputerDirectory::chdir(char const *path)
{
// ホストにあるディレクトリでないと chdir 不可。ただし、この仕様もどうかとは思う。
// そのため、仮想ディレクトリ指定済でないと不可
	try {
		DirectoryConverter c(this, localDirectory);
		char dir[1024];
		c.convert(path, dir);

#ifdef DEBUG
		fprintf(stderr, "!! RuputerDirectory::chdir path (%s) -> local path (%s) \n", path, dir);
		fflush(stderr);
#endif
		struct stat st;
		if(stat(dir, &st) == -1)
			return 1;

#ifdef MSVC
		if( ( st.st_mode & _S_IFDIR ) == 0 )
			return 1;
#else
		if(!S_ISDIR(st.st_mode))
			return 1;
#endif
	}
	catch( DirectoryConverterException &e ) {
// 仮想ディレクトリ未指定のため、chdir 不可
#ifdef DEBUG
		fprintf(stderr, "!! RuputerDirectory::chdir catch Exception");
		fflush(stderr);
#endif
		return 1;
	}

	DosDirectory::chdir(path);

	return 0;
}



void RuputerDirectory::getReplacePath(char *dst)
{
	strcpy(dst, "b:");
}


bool RuputerDirectory::isSetupDirectory(void)
{
	return true; // 常に設定されているとする
}
