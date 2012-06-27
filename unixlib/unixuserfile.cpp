/*
 * unixuserfile.cpp - Unix user defined file utility class
 *
 * $Id: unixuserfile.cpp,v 1.2 2006/02/09 17:58:39 fumi Exp $
 *
 * Copyright (C) 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

#include "unixuserfile.h"
#include "option.h"
#include "ufilerini.h"

#include "log.h"

UnixUserFile::UnixUserFile(Option *opt)
{
	this->opt = opt;
}

UnixUserFile::UnixUserFile(void)
{
	uini = new UFilerIni();
	opt = new Option();
}

UnixUserFile::~UnixUserFile(void)
{
	delete uini;
	delete opt;
}


bool UnixUserFile::load(void)
{
	homeDirectory = getenv("HOME");
	if( homeDirectory == NULL )
	{
		Log::getInstance()->output2(Log::Error, "Error!! environment value HOME is not defined.\n");
		return false;
	}

	char path[1024];

	strcpy(path, homeDirectory);
	strcat(path, "/.aurex/option.cfg");
	opt->load( path );

	strcpy(path, homeDirectory);
	strcat(path, "/.aurex/aurex.ini");
	uini->load( path );

	return true;
}

Option *UnixUserFile::getOption(void)
{
	return this->opt;
}

UFilerIni *UnixUserFile::getUFilerIni(void)
{
	return this->uini;
}


