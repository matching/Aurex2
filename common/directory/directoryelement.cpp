/*
 * directoryelement.cpp - Directory hieralkey(?)
 *
 * $Id: directoryelement.cpp,v 1.2 2004/11/12 19:06:11 fumi Exp $
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
#include <string.h>
#include <assert.h>

#include "dirstack.h"
#include "directoryelement.h"


DirectoryElement::DirectoryElement(void)
{
	directory=NULL;
}

DirectoryElement::DirectoryElement(char const *dir)
{
	int len;

	len=strlen(dir);
	directory=new char[len+1];
	memcpy(directory, dir, len);
	directory[len]=0;
}

DirectoryElement::DirectoryElement(char const *start, char const *end)
{
	int len;

	len=end-start;
	directory=new char[len+1];
	memcpy(directory, start, len);
	directory[len]=0;
}

DirectoryElement::~DirectoryElement(void)
{
	delete[] directory;
}

bool DirectoryElement::equal(DirectoryElement *dst)
{
	if(strcmp(directory, dst->directory) == 0)
		return true;
	return false;
}
