/*
 * dosdirectory.cpp - MS-DOS type directory module
 *
 * $Id: dosdirectory.cpp,v 1.17 2006/01/05 09:45:05 fumi Exp $
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
#include <string.h>
#include <assert.h>

#include "dirstacksjis.h"
#include "dosdirectory.h"
#include "codingsystem.h"

/**
 * Dos タイプのディレクトリ
 *
 */
DosDirectory::DosDirectory(void)
{
	dirstack = NULL;
	drive = 'c';

// onHand だと替える必要があるのかしら？
	coding = SHIFT_JIS;
}


DosDirectory::DosDirectory(char const *path)
{
	dirstack = NULL;
	drive = 'c';
	coding = SHIFT_JIS;

	setdir( path );
}

int DosDirectory::getRestPath(char const *src, char *restpath) throw(DirectoryException)
{
	assert( src );
	assert( restpath );

	DirStack *rest;

	if(*src == '\\')
	{
		DirStack *ds;

		ds=new DirStackSjis(src+1, '\\');
		ds->normalize();

		if(dirstack)
		{
			rest=dirstack->rest(ds);
			delete ds;
		}
		else
			rest=ds;
	}
	else if(*(src+1) == ':' && *(src+2) == '\\')
	{
		if(!isSameDriveLetter(*src))
			throw DirectoryException();

		DirStack *ds;
		ds=new DirStackSjis(src+3, '\\');

		ds->normalize();
		rest=dirstack->rest(ds);

		delete ds;
	}
	else
	{
#if 0
/* 仮想ルートディレクトリを設定してあれば？ */
		char str[1024];

		getdir(str);
		strcat(str, "\\");
		strcat(str, src);

		rest=new DirStackSjis(str+3, '\\');
#else
		rest=new DirStackSjis(src, '\\');
#endif
		rest->normalize();

	}

	if(rest == NULL)
	{
//		delete rest;
		throw DirectoryException();
	}
	else
		rest->toFullPath(restpath, '\\');

	delete rest;

	return 0;
}

bool DosDirectory::isAbsoluteDirectory(char const *path)
{
	if(*path == '\\')
		return true;
	if(*(path+1) == ':' && *(path+2) == '\\')
		return true;
	return false;
}

int DosDirectory::chdir(char const *path)
{
	if(*path == 0)
		return 0;

	if(*path == '\\')
	{
		delete dirstack;
		dirstack=new DirStackSjis(path+1, '\\');
	}
	else if(*(path+1) == ':')
	{
		if(*(path+2) == '\\')
		{
			delete dirstack;
			drive=*path;
			dirstack=new DirStackSjis(path+3, '\\');
		}
		else
			drive=*path;
	}
	else
	{
		DirStack *ch;
		ch=new DirStackSjis(path, '\\');
		if(dirstack)
		{
			dirstack->concat(ch);
			delete ch;
		}
		else
			dirstack=ch;
	}

	if(dirstack)
		dirstack->normalize();
	return 0;
}

int DosDirectory::getdir(char *path)
{
	if(dirstack == NULL)
	{
		path[0]=drive;
		path[1]=':';
		path[2]=0;
		return 0;
	}

	dirstack->toFullPath(path+2, '\\');
	*path=drive;
	*(path+1)=':';

	return 0;
}

/**
 * 絶対パスのみ対応
 */
void DosDirectory::setdir(char const *path)
{
	if( *path == 0 )
	{
		delete dirstack;
		dirstack = NULL;
		return;
	}

	if(*(path+1) == ':' && *(path+2) == '\\')
	{
		drive = *path;
		dirstack = new DirStackSjis(path+3, '\\');
	}

	if(dirstack)
		dirstack->normalize();
}

int DosDirectory::getPathDelimiter(void)
{
	return '\\';
}



void DosDirectory::caseConvertion(char *path)
{
// do nothing
}



bool DosDirectory::isSetupDirectory(void)
{
	return Directory::isSetupDirectory();
}

bool DosDirectory::isSameDriveLetter(char dst_drive)
{
	char big_drive;
	char big_dst_drive;

	if(drive >= 'a' && drive <= 'z')
		big_drive=drive-'a'+'A';
	else
		big_drive=drive;

	if(dst_drive >= 'a' && dst_drive <= 'z')
		big_dst_drive=dst_drive-'a'+'A';
	else
		big_dst_drive=dst_drive;

	return big_drive == big_dst_drive;
}


void DosDirectory::getReplacePath(char *dst)
{
	getdir(dst);
}


Directory *DosDirectory::createDirectory(DirStack *root)
{
	DosDirectory *d = new DosDirectory();

	d->drive = this->drive;
	d->dirstack = root;

	return d;
}


void DosDirectory::splitDirFile(char const *src, char *dir, char *file)
{

	Directory::splitDirFile(src, dir, file);

	char *p;


	p = strchr( dir, 0 );
	if( *( p - 1 ) == ':' )
	{
		*p = '\\';
		*(p + 1) = 0;
	}
}
