/*
 * main.cpp - windows main
 *
 * $Id: main.cpp,v 1.37 2006/02/10 14:10:57 fumi Exp $
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



#include "winruputer.h"
#include "winframe.h"
#include "winsound.h"
#include "winoption.h"

#include "dosdirectory.h"
#include "ufilerini.h"

#include "log.h"
#include "logoutputstderr.h"

#include <windows.h>
#include <stdio.h>


#define MUTEX_NAME "aurex2_mutex"

#define OPTION_NAME "option.cfg"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR lpszCmdLine, int nCmdShow)
{
	WINRuputer *obj;
	WINOption *opt = new WINOption();
	HANDLE hMutex = NULL;
	Frame *frame = new WINFrame();

	opt->load(OPTION_NAME);

	if( opt->get_only_a_process() )
	{
		hMutex = CreateMutex(NULL, FALSE, MUTEX_NAME);
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			HWND hOldWnd;
			COPYDATASTRUCT cds;

			hOldWnd = FindWindow( CLASS_NAME, NULL );
			if( hOldWnd )
			{
				if( *lpszCmdLine != 0 )
				{
					cds.dwData = 100;/* 適当？ */
					cds.cbData = strlen( lpszCmdLine ) + 1;
					cds.lpData = lpszCmdLine;

					SendMessage(hOldWnd, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds);
				}
		// 最前面にはしておく
				SetForegroundWindow(hOldWnd);
			}

			return 0;
		}
	}


	UFilerIni *ini = new UFilerIni();
	ini->load("aurex.ini");

	DosDirectory *dosDir = new DosDirectory(opt->get_root_directory());
	if( dosDir->isSetupDirectory() && ! dosDir->isExistentDirectory() )
	{
		char str[1024];
		dosDir->getdir(str);
		Log::getInstance()->output(Log::Warning, "Warning ! Directory %s is not exists\n", str);
	}


// Todo この辺のシーケンス修正
	obj=new WINRuputer(hInstance,
					   nCmdShow,
					   frame,
					   dosDir,
			   //					   new WINSound(),
					   opt,
					   ini);

//		obj->loadOption();
//		obj->loadFilerIni();

	if(*lpszCmdLine == 0)
	{
#if 0
		char filename[256]="";
		obj->getFileName(NULL, filename);
		obj->load(filename);
#endif
	}
	else
		obj->load(lpszCmdLine);
	obj->main_loop();

	opt->syncToOptionList();
	opt->save(OPTION_NAME);

	delete frame;
	delete dosDir;
	delete ini;
	delete opt;
	delete obj;

	if( hMutex )
		CloseHandle(hMutex);

	Log::getInstance()->freeLogOutput();

	return 0;
}
