/*
 * main.cpp - curses main
 *
 * $Id: main.cpp,v 1.10 2006/02/20 13:58:05 fumi Exp $
 *
 * Copyright (C) 2004, 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

#include <string.h>
#include <unistd.h>

#include "curruputer.h"

#include "unixframe.h"
#include "unixuserfile.h"

#include "unixdirectory.h"
#include "option.h"

#include "log.h"

int main(int argc, char **argv)
{
	if( argc <= 1 )
	{
		fprintf(stderr, "Error.\n Usage: %s filename\n", argv[0]);
		return 0;
	}

	UnixUserFile userFile;

	if( ! userFile.load() )
	{
		return 1;
	}

/*
 * setup widget depending option
 */


	UnixDirectory *dir = new UnixDirectory( userFile.getOption()->get_root_directory() );
	if( dir->isSetupDirectory() && ! dir->isExistentDirectory() )
	{
		char str[1024];
		dir->getdir(str);
		Log::getInstance()->output(Log::Warning, "Warning ! Directory %s is not exists\n", str);
	}

	CURRuputer *obj;
	Frame *frame = new UnixFrame();

	obj=new CURRuputer(frame,
					   dir,
					   userFile.getOption(),
					   userFile.getUFilerIni());

	try{
		obj->load(argv[1]);
	}
	catch(...)
	{
		delete obj;

		fprintf(stderr, "Error has occured. Please see log.");
		return 0;
	}

	obj->main_loop();

	Log::getInstance()->freeLogOutput();

	delete obj;

	delete dir;
	delete frame;

	return 0;
}
