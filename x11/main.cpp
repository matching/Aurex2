/*
 * main.cpp - X11 main
 *
 * $Id: main.cpp,v 1.14 2006/02/19 13:04:40 fumi Exp $
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

#include <signal.h>

#include "unixframe.h"
#include "unixuserfile.h"

#include "x11ruputer.h"
#include "x11exception.h"

#include "option.h"
#include "unixdirectory.h"
#include "log.h"
#include "logoutputstderr.h"

static void signal_handler_sigusr1(int signo);

static Option *opt = NULL;

int main(int argc, char **argv)
{
	int res;

	if(argc <= 1)
		return 0;

	X11Ruputer *obj;
	UnixUserFile userFile;
	Frame *frame = new UnixFrame();

	if( ! userFile.load() )
	{
		Log::getInstance()->freeLogOutput();
		return 1;
	}

	UnixDirectory *dir = new UnixDirectory( userFile.getOption()->get_root_directory() );
	if( dir->isSetupDirectory() && ! dir->isExistentDirectory() )
	{
		char str[1024];
		dir->getdir(str);
		Log::getInstance()->output(Log::Warning, "Warning ! Directory %s is not exists\n", str);
	}

	try {
		obj = new X11Ruputer(argc, argv,
							 frame,
							 dir,
							 userFile.getOption(),
							 userFile.getUFilerIni());
	}
	catch( X11DisplayOpenException &e )
	{
		Log::getInstance()->output2(Log::Error, "Display open error\n");
		Log::getInstance()->freeLogOutput();
		return 1;
	}

	try{
		res = obj->load( argv[1] );
	}
	catch(...)
	{
		delete obj;

		fprintf(stderr, "Error has occured. Please see log.");
		return 0;
	}

	obj->main_loop();


	Log::getInstance()->freeLogOutput();

	delete dir;
	delete frame;
	delete obj;

	return 0;
}



static void signal_handler_sigusr1(int signo)
{
	char const *home_dir;
	char path[1024];

	home_dir = getenv("HOME");
	strcpy(path, home_dir);
	strcat(path, "/.aurex/option.cfg");
	opt->save( path );
}
