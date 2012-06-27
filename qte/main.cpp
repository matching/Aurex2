/*
 * main.cpp - Qt/E main
 *
 * $Id: main.cpp,v 1.17 2006/02/19 03:01:35 fumi Exp $
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

#include "unixframe.h"
#include "unixuserfile.h"

#include "option.h"
#include "ufilerini.h"
#include "state.h"

#include "qteruputer.h"

#include <qapplication.h>
#include <qpe/qpeapplication.h>
#include <qrect.h>
#include <qdirectpainter_qws.h>
#include <qtextcodec.h>

#include <pthread.h>

#include "unixdirectory.h"
#include "log.h"
#include "logoutputstderr.h"

int main(int argc, char **argv)
{
	int res;

	QPEApplication a(argc, argv);

	QTERuputer *obj;
	pthread_t mythread;

	UnixUserFile userFile;
	Frame *frame = new UnixFrame();

	if( ! userFile.load() )
	{
		Log::getInstance()->freeLogOutput();
		return 1;
	}

	a.setDefaultCodec( QTextCodec::codecForName("eucJP") );

	UnixDirectory *dir = new UnixDirectory( userFile.getOption()->get_root_directory() );

	obj=new QTERuputer(frame,
					   dir,
					   userFile.getOption(),
					   userFile.getUFilerIni());


	if( argc > 1 )
	{
		obj->load(argv[1]);
	}
	a.setMainWidget(obj);

	obj->show();
	obj->setCaption("Aurex"); //

	pthread_create(&mythread, NULL, obj->thread_func, obj);
	a.exec();

	pthread_join(mythread, NULL);


#if 0
	{
		QDirectPainter p(obj);
		QRect r(0, 0, 640, 480);
//		p.setAreaChanged(r);
	}
#endif

	delete dir;
	delete frame;
	delete obj;

	Log::getInstance()->freeLogOutput();

	return 0;
}
