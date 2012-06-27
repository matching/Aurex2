/*
 * directory.cpp - directory
 *
 * $Id: directory.cpp,v 1.10 2006/01/05 17:24:44 fumi Exp $
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

#include "directory.h"
#include "dirstack.h"

#include "codingbase.h"
#include "codingascii.h"
#include "codingshiftjis.h"

#include "codingsystem.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <assert.h>

/**
 * コンストラクタ
 */
Directory::Directory(void)
{
	dirstack=0;
	coding = ASCII;
}

Directory::~Directory(void)
{
	delete dirstack;
}


bool Directory::isSetupDirectory(void)
{
	if(dirstack == 0)
		return false;

	return dirstack->isSetup();
}

bool Directory::isExistentDirectory(void)
{
	char path[1024];
	struct stat st;

	getdir( path );

	if(stat(path, &st) == -1)
		return false;

#ifdef MSVC
	if( st.st_mode & _S_IFDIR )
		return true;
#else
	if( S_ISDIR( st.st_mode ) )
		return true;
#endif

	return false;
}


void Directory::splitDirFile(char const *src, char *dir, char *file)
{
	assert( src );
	assert( dir );
	assert( file );

	CodingBase *code;
	int i;

	code = this->createCoding( src );
	i = code->searchReverse( getPathDelimiter() );

	code->subString(0, i, dir);
	code->subString(i + 1, -1, file);

	delete code;

	return;
}

#if 0
/**
 *
 */
void Directory::getFullFileName(char const *filename, char *fullFileName) throw(DirectoryUndefinedExcption)
{
	if( isAbsoluteDirectory(filename) )
	{
		strcpy(fullFileName, filename);
		return;
	}

#if 0
	fprintf(stderr, "isAbsoluteDirectory(%s) return %d\n", filename, isAbsoluteDirectory(filename));
#endif

	if( ! isSetupDirectory() )
		throw DirectoryUndefinedExcption();


	{
		DirStack *rootDir, *file;
		char pathDelim = getPathDelimiter();

		rootDir = dirstack->clone();
		file = new DirStack(filename, pathDelim);
#if 0
		char buf[1024];
		file->toFullPath(buf, '/');
		fprintf(stderr, "debug file: %s\n", buf);
		fflush(stderr);

		rootDir->toFullPath(buf, '/');
		fprintf(stderr, "debug clone: %s\n", buf);
		fflush(stderr);
#endif

		rootDir->concat(file);
		rootDir->normalize();
		Directory *dir = createDirectory(rootDir);
		dir->getdir(fullFileName);

		delete dir;
		delete file;
//		delete rootDir; // dir のデストラクタで解放
	}

}
#endif

enum CodingSystem Directory::getCodingSystem(void)
{
	return coding;
}


CodingBase *Directory::createCoding(char const *str)
{
	switch( coding )
	{
	case ASCII:
	case ISO_8859_1:
	case EUC_JP: // ?
		return new CodingAscii(str);
		break;
	case SHIFT_JIS:
		return new CodingShiftJis(str);
	}
	return NULL;
}

