/* -*-mode:c++; coding:euc-jp-unix-*-
 * qteruputer_load.cpp - Qt/E library ruputer emulation for load (enable exception)
 *
 * $Id: qteruputer_load.cpp,v 1.4 2006/02/20 08:20:02 fumi Exp $
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

#include <stdio.h>
#include <qmessagebox.h>

#include "qteruputer.h"

int QTERuputer::load(char const *filename) throw()
{
	QMessageBox *qmb = NULL;
	char message[256] = "";

	try{
		Ruputer::load(filename);
	}
	catch( ExfFileInvalidException &e ) {
		sprintf(message, "exf file is invalid (%s)", filename);
	}
	catch( ExfFileNotFoundException &e ) {
		sprintf(message, "A exf file associated with (%s) is not found", filename);
	}
	catch( AssociatedFileNotInVirtualDirectoryException &e ) {
		sprintf(message, "A file (%s) is out of virtual directory.", filename);
	}

	if( *message )
	{
		qmb = new QMessageBox("Load Error", message, QMessageBox::NoIcon, QMessageBox::Ok, 0, 0, NULL /* no parent */, 0 /* no name */, false /* not modal (modeless) */);
		qmb->show();
		delete qmb;
	}

	return 0;
}
