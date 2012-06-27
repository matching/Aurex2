/* -*-mode:c++ coding:euc-jp-unix-*-
 *
 * qtefiledialog.cpp - Qt/E library ruputer emulation
 *
 * $Id: qtefiledialog.cpp,v 1.2 2006/02/12 17:17:22 fumi Exp $
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


#include "qteruputer.h"
#include <qlayout.h>

#ifdef FILEDIALOG
#include <sl/slfileselector.h>
#endif

#include "qtefiledialog.h"
#include "directory.h"

QTEFileDialog::QTEFileDialog(QTERuputer *parent) : QDialog(parent, "Aurex executable file", true /* modal */)
{
#ifdef FILEDIALOG
	char dir[1024] = "";

	if( parent->getLocalDirectory()->isSetupDirectory() )
	{
		parent->getLocalDirectory()->getdir(dir);
	}

	slFileSelector = new SlFileSelector( dir, "*/*", this, "slfileselector" );

	boxLayout = new QVBoxLayout( this );
	boxLayout->addWidget( slFileSelector );

	showMaximized();

	selectFileName = NULL;
#endif
}

QTEFileDialog::~QTEFileDialog(void)
{
#ifdef FILEDIALOG
	delete boxLayout;
	delete slFileSelector;
#endif
}

void QTEFileDialog::accept()
{
#ifdef FILEDIALOG
	selectFileName = slFileSelector->currentFile();
	emit done( Accepted );
#endif
}

char const *QTEFileDialog::getSelectFileName(void)
{
	return selectFileName;
}
