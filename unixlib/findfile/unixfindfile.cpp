/*
 * unixfindfile.cpp - findfile emulation for unix using by glob
 *
 * $Id: unixfindfile.cpp,v 1.3 2006/02/22 17:40:39 fumi Exp $
 *
 * Copyright (C) 2004, 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "glob.h"
#include "unixfindfile.h"

UnixFindFile::UnixFindFile(struct dta_t *dta_area, char const *pathname, unsigned char search_attribute) : SDFindFile(dta_area, pathname, search_attribute)
{
// do nothing

}

UnixFindFile::~UnixFindFile(void)
{
	globfree(&glob_data);
}


/**
 * @retval 0 正常
 * @retval != 0 異常（この値が DOSerr 値になる）
 *         2 ファイルが見つからない
 *         3 パスが見つからない
 *        18 もうファイルが無い。見つからない（2とどう違うんだ？）
 */
int UnixFindFile::findFirst(void)
{
	int ret;

	glob_index=0;

	memset(&glob_data, 0, sizeof(glob_t));

	ret=glob(pathname, 0, NULL, &glob_data);
	if(ret != 0)
	{
 // error
		return 2;
	}

	ret=processNextElement();
	if(ret != 0)
		return 2;
	return 0;
}

int UnixFindFile::findNext(void)
{
	int ret;

	ret=processNextElement();
	if(ret != 0)
		return 2;
	return 0;
}

int UnixFindFile::processNextElement(void)
{
	char name[256];
	int ret;
	struct stat stat_data;

	ret=pickupNextElement(name, &stat_data);
	if(ret != 0)
		return 1;

	dta_area->size=stat_data.st_size;
	dta_area->attrib=to_ruputer_attrib(stat_data.st_mode);

	set_file_time(stat_data.st_mtime);


// EUC なら OK?
	{
		char *p;

		p=strrchr(name, '/');
		if(p)
			strncpy(dta_area->filename, p+1, sizeof(dta_area->filename));
		else
			strncpy(dta_area->filename, name, sizeof(dta_area->filename));

		dta_area->filename[ sizeof( dta_area->filename ) -1 ] = 0;
	}

	return 0;
}

int UnixFindFile::pickupNextElement(char *name, struct stat *stat_data)
{
	int ret;

	for(;; glob_index++ )
	{
		if(glob_data.gl_pathv[glob_index] == NULL)
			return 1;

		ret=stat(glob_data.gl_pathv[glob_index], stat_data);
		if(ret != 0)
			continue;

		if(is_mokuteki_attrib(stat_data->st_mode))
			break;
	}

	strcpy(name, glob_data.gl_pathv[glob_index]);
	glob_index++;

	return 0;
}



// ToDo ダメダメ
int UnixFindFile::is_mokuteki_attrib(mode_t mode)
{
	int i;

// ディレクトリも対象だとすれば全てOK
	if(search_attribute & 0x10)
		return 1;

	if(mode & S_IFDIR)
		return 0;

	return 1;
}


unsigned char UnixFindFile::to_ruputer_attrib(mode_t mode)
{
	if(mode & S_IFDIR)
		return 0x10;

	return 0;
}

void UnixFindFile::set_file_time(time_t mtime)
{
	struct tm fileTm;

	localtime_r(&mtime, &fileTm);

	setDate(fileTm.tm_year + 1900, fileTm.tm_mon + 1, fileTm.tm_mday);
	setTime(fileTm.tm_hour, fileTm.tm_min, fileTm.tm_sec);
}
