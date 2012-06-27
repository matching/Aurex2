/*
 * directoryconverter.cpp - Convert Ruputer's directory
 *
 * $Id: directoryconverter.cpp,v 1.19 2006/01/09 07:43:14 fumi Exp $
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

#include "directoryconverter.h"
#include "directory.h"

#include "codingbase.h"
#include "log.h"

/**
 * ディレクトリ変換クラス
 * @param from 変換元タイプ
 * @param to 変換先タイプ
 */
DirectoryConverter::DirectoryConverter(Directory *from, Directory *to) throw(DirectoryConverterException)
{
	if( from == NULL || to == NULL
		|| ! from->isSetupDirectory() || ! to->isSetupDirectory() )
	{
#ifdef DEBUG
		fprintf(stderr, "!! DirectoryConverter::DirectoryConvertor from:%p, to:%p", from, to);
		if( from )
			fprintf(stderr, "!! DirectoryConverter::DirectoryConvertor from: %d", from->isSetupDirectory());
		if( to )
			fprintf(stderr, "!! DirectoryConverter::DirectoryConvertor   to: %d", to->isSetupDirectory());

		fflush(stderr);
#endif
		throw DirectoryConverterException();
	}

	this->from = from;
	this->to = to;
}

/**
 * パス変換メソッド
 */
int DirectoryConverter::convert(char const *src, char *dst)  throw(DirectoryConverterException)
{
	char restpath[256];

#ifdef DEBUG
	fprintf(stderr, "!! DirectoryConverter::convert to (%d) %s %d \n", to->isSetupDirectory(), src, true);
	fflush(stderr);
#endif

	if(to->isSetupDirectory() == false)
		throw DirectoryConverterException();
/*
 * src から、残りのエレメントを取得する
 */
	try {
		from->getRestPath(src, restpath);
#ifdef DEBUG
		fprintf(stderr, "!! DirectoryConverter::convert rest path (%s)\n", restpath);
		fflush(stderr);
#endif

		to->caseConvertion(restpath);

/**
 * 変換先パスの先頭〜の文字列を取得
 * to が RuputerDirectory の場合は "b:" を取得
 * to が Dos, Unix Directory の場合は仮想ルートディレクトリを取得
 */
		to->getReplacePath(dst);
	}
	catch( DirectoryException &e )
	{
		throw DirectoryConverterException();
	}

	strcat(dst, restpath);
// コーディングの変換も必要？
	replacePathDelimiter(dst);


	Log::getInstance()->output(Log::Info, "Directory convert \"%s\" -> \"%s\"\n", src, dst );

#ifdef DEBUG
	{
		char path[256]="unkonwn";

//		::ruputer->rootdir->getdir(path);
		fprintf(stderr, "!! DirectoryConverter::convert root (%s) dir convert (%s) -> (%s)\n", path, src, dst);
		fflush(stderr);
	}
#endif

	return 0;
}

/**
 * パスデリミタの置き換え
 * str は from と同様のタイプであること
 */
void DirectoryConverter::replacePathDelimiter(char *str)
{
	int d, s;

	s=from->getPathDelimiter();
	d=to->getPathDelimiter();

	if(s == d)
		return;

	CodingBase *code;

	code = from->createCoding( str );
	code->replaceCharacter( s, d );
	code->toString( str );

	delete code;
}
