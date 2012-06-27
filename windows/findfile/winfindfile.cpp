/*
 * winfindfile.cpp - windows fnd_first, next emulation
 *
 * $Id: winfindfile.cpp,v 1.4 2006/03/06 12:58:17 fumi Exp $
 *
 * Copyright (C) 2004 2005, 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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
#include <windows.h>

#include "winfindfile.h"

/*
#define FILE_ATTRIBUTE_READONLY 		0x01
#define FILE_ATTRIBUTE_HIDDEN   		0x02
#define FILE_ATTRIBUTE_SYSTEM   		0x04
#define FILE_ATTRIBUTE_DIRECTORY        0x10
#define FILE_ATTRIBUTE_ARCHIVE 			0x20
#define FILE_ATTRIBUTE_ENCRYPTED        0x30
#define FILE_ATTRIBUTE_NORMAL   		0x40
#define FILE_ATTRIBUTE_TEMPORARY       0x100
#define FILE_ATTRIBUTE_SPARSE_FILE     0x200
#define FILE_ATTRIBUTE_REPARSE_POINT   0x400
#define FILE_ATTRIBUTE_COMPRESSED      0x800
#define FILE_ATTRIBUTE_OFFLINE  		0x1000
#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED      0x2000
*/

DWORD const WINFindFile::win_attrib_tbl[]={
	FILE_ATTRIBUTE_READONLY,
	FILE_ATTRIBUTE_HIDDEN,
	FILE_ATTRIBUTE_SYSTEM,
//	FILE_ATTRIBUTE_NORMAL,	/* ?? */
	0,
	FILE_ATTRIBUTE_DIRECTORY,
	FILE_ATTRIBUTE_ARCHIVE
};



WINFindFile::WINFindFile(struct dta_t *dta_area, char const *pathname, unsigned char search_attribute) : SDFindFile(dta_area, pathname, search_attribute)
{
// do nothing

}

WINFindFile::~WINFindFile(void)
{
	if(hFind)
		CloseHandle(hFind);
}


/**
 * @retval 0 正常
 * @retval != 0 異常（この値が DOSerr 値になる）
 *         2 ファイルが見つからない
 *         3 パスが見つからない
 *        18 もうファイルが無い。見つからない（2とどう違うんだ？）
 */
int WINFindFile::findFirst(void)
{
	int res;
	WIN32_FIND_DATA find_data;

	hFind=FindFirstFile(pathname, &find_data);
	if( hFind == INVALID_HANDLE_VALUE )
		return 2;

	for(;;)
	{
		if(is_mokuteki_attrib(find_data.dwFileAttributes, search_attribute))
			break;
		res=FindNextFile(hFind, &find_data);
		if(res == 0)	/* fault */
			return 18;
	}


/* find_data 目的のファイル */
	dta_area->size=find_data.nFileSizeLow;
	dta_area->attrib=to_ruputer_attrib(find_data.dwFileAttributes);

	set_file_time(&find_data);

	if(strlen(find_data.cFileName) < 13)
		strcpy(dta_area->filename, find_data.cFileName);
	else
		memcpy(dta_area->filename, find_data.cAlternateFileName, 13);

	return 0;
}

int WINFindFile::findNext(void)
{
	int res;
	WIN32_FIND_DATA find_data;

	do {
		res=FindNextFile(hFind, &find_data);

		if(res == 0)	/* fault */
			return 2; // ToDo 値を調べる
	}while(!is_mokuteki_attrib(find_data.dwFileAttributes, search_attribute));


/* find_data 目的のファイル */
	dta_area->size=find_data.nFileSizeLow;
	dta_area->attrib=to_ruputer_attrib(find_data.dwFileAttributes);

	set_file_time(&find_data);

	if(strlen(find_data.cFileName) < 13)
		strcpy(dta_area->filename, find_data.cFileName);
	else
		memcpy(dta_area->filename, find_data.cAlternateFileName, 13);

	return 0;
}

unsigned char WINFindFile::to_ruputer_attrib(DWORD attrib)
{
	int i;
	unsigned char rup_attrib=0;

	for(i=0; i < 6; i++)
		if(attrib & win_attrib_tbl[i])
			rup_attrib|=1 << i;

	return rup_attrib;
}

int WINFindFile::is_mokuteki_attrib(DWORD attrib, int rup_attrib)
{
	int i;

	for(i=0; i < 6; i++)
		if((rup_attrib >> i) & 1)
			if(attrib & win_attrib_tbl[i])
				return 1;

	return 0;
}



void WINFindFile::set_file_time(WIN32_FIND_DATA const *find_data)
{
	FILETIME local_file_time;
	SYSTEMTIME system_time;

	FileTimeToLocalFileTime(&find_data->ftLastWriteTime, &local_file_time);
	FileTimeToSystemTime(&local_file_time, &system_time);

	setDate(system_time.wYear, system_time.wMonth, system_time.wDay);
	setTime(system_time.wHour, system_time.wMinute, system_time.wSecond);

}
