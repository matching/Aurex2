/* -*-mode:c++ ; coding:euc-jp-*-
 *
 * unixdirectory.cpp - Unix type directory module
 *
 * $Id: unixdirectory.cpp,v 1.14 2006/01/05 09:45:05 fumi Exp $
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

#include "dirstack.h"
#include "unixdirectory.h"
#include "codingsystem.h"

/**
 * Unix タイプのディレクトリクラス
 */
UnixDirectory::UnixDirectory(void)
{
	coding = EUC_JP;
}

UnixDirectory::UnixDirectory(char const *path)
{
	coding = EUC_JP;

	setdir(path);
}

int UnixDirectory::getRestPath(char const *src, char *restpath) throw(DirectoryException)
{
	DirStack *rest;

	if(*src == '/')
	{
		DirStack *ds;
		ds=new DirStack(src+1, '/');
		ds->normalize();

		if(dirstack)
		{
			rest=dirstack->rest(ds);
			delete ds;
		}
		else
			rest=ds;
	}
	else
	{
#if 0
		char str[1024];

		getdir(str);
		strcat(str, "/");
		strcat(str, src);

		rest=new DirStack(str, '/');
#else
		rest=new DirStack(src, '/');
#endif
		rest->normalize();
	}

	if(rest == NULL)
	{
// delete rest
		throw DirectoryException();
	}
	else
		rest->toFullPath(restpath, '/');

	delete rest;

	return 0;
}

void UnixDirectory::caseConvertion(char *path)
{
	while(*path)
	{
		if(*path >= 'A' && *path <= 'Z')
			*path-='A'-'a';
		path++;
	}
}



bool UnixDirectory::isAbsoluteDirectory(char const *path)
{
	if(*path == '/')
		return true;
	return false;
}

int UnixDirectory::chdir(char const *path)
{
	if(*path == '/')
	{
		delete dirstack;
		dirstack=new DirStack(path+1, '/');
	}
	else
	{
		DirStack *ch;
		ch=new DirStack(path, '/');
		if(dirstack)
		{
			dirstack->concat(ch);
			delete ch;
		}
		else
			dirstack=ch;

	}
	dirstack->normalize();
	return 0;
}

int UnixDirectory::getdir(char *path)
{
	if(dirstack == NULL)
	{
		path[0]='/';
		path[1]=0;
		return 0;
	}
	dirstack->toFullPath(path, '/');
	return 0;
}

void UnixDirectory::setdir(char const *path)
{
	if( *path == 0 )
	{
		delete dirstack;
		dirstack = NULL;
		return;
	}

	dirstack = new DirStack(path, '/');
}

int UnixDirectory::getPathDelimiter(void)
{
	return '/';
}



void UnixDirectory::getReplacePath(char *dst)
{
	dirstack->toFullPath(dst, '/');
}

bool UnixDirectory::isSetupDirectory(void)
{
	return Directory::isSetupDirectory();
}

Directory *UnixDirectory::createDirectory(DirStack *root)
{
	UnixDirectory *d = new UnixDirectory();

	d->dirstack = root;
	return d;
}
