/* -*-mode:c++; coding:euc-jp-unix-*-
 * qteruputer.cpp - Qt/E library ruputer emulation
 *
 * $Id: qteruputer.cpp,v 1.18 2006/02/20 08:16:20 fumi Exp $
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


#include "qteruputer.h"
#include "qtelcd.h"
#include "qtefiledialog.h"

#include "keybuffer.h"
#include "state.h"
#include "rupaddress.h"
#include "cmlcd.h"
#include "log.h"
#include "frame.h"

#include <qpainter.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qaction.h>
#include <qpixmap.h>
#include <qiconset.h>

#include <stdio.h>
#include <pthread.h>

#include "unixfindfile.h"


#include "image/up-arrow.xpm"
#include "image/down-arrow.xpm"
#include "image/right-arrow.xpm"
#include "image/left-arrow.xpm"

#include "image/enter.xpm"
#include "image/el.xpm"
#include "image/menu.xpm"
#include "image/filer.xpm"

pthread_mutex_t eventmutex=PTHREAD_MUTEX_INITIALIZER;

QTERuputer::QTERuputer(Frame *_frame, Directory *_sddir, Option *option, UFilerIni *ini) : QMainWindow(NULL, "Aurex"), Ruputer(_frame, _sddir, option, ini)
{
//	qteruputer=this;
//	palette = NULL;

#if 1
/* configure menu */
	openFileMenu = new QPopupMenu(this);
	menuBar()->insertItem(tr("File"), openFileMenu);

	openAction = new QAction("Open", "Open", 0, this);
	openAction->addTo(openFileMenu);
	connect(openAction, SIGNAL(activated()), this, SLOT(openFile(void)));

	connect(openFileMenu, SIGNAL(aboutToShow()), this, SLOT(pauseEmulate(void)));
	connect(openFileMenu, SIGNAL(aboutToHide()), this, SLOT(resumeEmulate(void)));

/* configure tool button */
	enterAction = new QAction("enter", "enter", 0, this);
	QPixmap enterPixmap( (const char **)enter );
	QIconSet enterIconSet( enterPixmap, QIconSet::Small );
	enterAction->setIconSet(enterIconSet);

	elAction = new QAction("el", "el", 0, this);
	QPixmap elPixmap( (const char **)el );
	QIconSet elIconSet( elPixmap , QIconSet::Small );
	elAction->setIconSet(elIconSet);

	menuAction = new QAction("menu", "menu", 0, this);
	QPixmap menuPixmap( (const char **)menu );
	QIconSet menuIconSet( menuPixmap , QIconSet::Small);
	menuAction->setIconSet(menuIconSet);

	filerAction = new QAction("filer", "filer", 0, this);
	QPixmap filerPixmap( (const char **)filer );
	QIconSet filerIconSet( filerPixmap , QIconSet::Small );
	filerAction->setIconSet(filerIconSet);


	upAction = new QAction("up", "up", 0, this);
	QPixmap upPixmap( (const char **)up_arrow_xpm );
	QIconSet upIconSet( upPixmap , QIconSet::Small );
	upAction->setIconSet(upIconSet);

	rightAction = new QAction("right", "right", 0, this);
	QPixmap rightPixmap( (const char **)right_arrow_xpm );
	QIconSet rightIconSet( rightPixmap , QIconSet::Small );
	rightAction->setIconSet(rightIconSet);

	leftAction = new QAction("left", "left", 0, this);
	QPixmap leftPixmap( (const char **)left_arrow_xpm );
	QIconSet leftIconSet( leftPixmap , QIconSet::Small );
	leftAction->setIconSet(leftIconSet);

	downAction = new QAction("down", "down", 0, this);
	QPixmap downPixmap( (const char **)down_arrow_xpm );
	QIconSet downIconSet( downPixmap , QIconSet::Small );
	downAction->setIconSet(downIconSet);

	connect(enterAction, SIGNAL(activated()), this, SLOT(pushPaletteEnter(void)));
	connect(elAction, SIGNAL(activated()), this, SLOT(pushPaletteEl(void)));
	connect(menuAction, SIGNAL(activated()), this, SLOT(pushPaletteMenu(void)));
	connect(filerAction, SIGNAL(activated()), this, SLOT(pushPaletteFiler(void)));
	connect(upAction, SIGNAL(activated()), this, SLOT(pushPaletteUp(void)));
	connect(downAction, SIGNAL(activated()), this, SLOT(pushPaletteDown(void)));
	connect(leftAction, SIGNAL(activated()), this, SLOT(pushPaletteLeft(void)));
	connect(rightAction, SIGNAL(activated()), this, SLOT(pushPaletteRight(void)));

	palette = new QToolBar(this, "Palette");
//	palette->setVerticalStretchable( true );
//	palette->setHorizontalStretchable( true );

	enterAction->addTo(palette);
	menuAction->addTo(palette);
	upAction->addTo(palette);
	downAction->addTo(palette);
	leftAction->addTo(palette);
	rightAction->addTo(palette);

	elAction->addTo(palette);
	filerAction->addTo(palette);
	setDockEnabled(palette, Bottom, true);
	moveToolBar(palette, Bottom);

	qtelcd=new QTELcd(getSystemAddress(), this, getCMLcd(), menuBar()->height(), palette->height(), palette->width());
	setSDLcd(qtelcd);
#endif
	setCaption("Aurex");
}

QTERuputer::~QTERuputer(void)
{
#if 0
	delete palette;
	delete enterAction;
	delete elAction;
	delete menuAction;
	delete filerAction;
	delete upAction;
	delete downAction;
	delete rightAction;
	delete leftAction;

	delete openAction;
	delete openFileMenu;
#endif

	delete qtelcd;
}


int QTERuputer::EventProcedure(void)
{
	event_return_value = 0;

	pthread_mutex_unlock(&eventmutex);

	pthread_mutex_lock(&eventmutex);

    return event_return_value;
}

// 以下 SLOT の実装

void QTERuputer::pauseEmulate(void)
{
	pthread_mutex_lock(&eventmutex);
}

void QTERuputer::resumeEmulate(void)
{
	pthread_mutex_unlock(&eventmutex);
}


/**
 * 同期化しなくても queue の ++ は最後に行なわれるので問題ない。
 */
void QTERuputer::pushPaletteEnter(void)
{
	keybuf->enQueueKey(Bkey_B | KEY_PUSH_DOWN);
	keybuf->enQueueKey(Bkey_B | KEY_PUSH_UP);
}

void QTERuputer::pushPaletteMenu(void)
{
	keybuf->enQueueKey(Bkey_D | KEY_PUSH_DOWN);
	keybuf->enQueueKey(Bkey_D | KEY_PUSH_UP);
}

void QTERuputer::pushPaletteEl(void)
{
	keybuf->enQueueKey(Bkey_C | KEY_PUSH_DOWN);
	keybuf->enQueueKey(Bkey_C | KEY_PUSH_UP);
}

void QTERuputer::pushPaletteFiler(void)
{
	keybuf->enQueueKey(Bkey_A | KEY_PUSH_DOWN);
	keybuf->enQueueKey(Bkey_A | KEY_PUSH_UP);
}

void QTERuputer::pushPaletteUp(void)
{
	keybuf->enQueueKey(Bkey_up | KEY_PUSH_DOWN);
	keybuf->enQueueKey(Bkey_up | KEY_PUSH_UP);
}

void QTERuputer::pushPaletteDown(void)
{
	keybuf->enQueueKey(Bkey_dw | KEY_PUSH_DOWN);
	keybuf->enQueueKey(Bkey_dw | KEY_PUSH_UP);
}

void QTERuputer::pushPaletteLeft(void)
{
	keybuf->enQueueKey(Bkey_lf | KEY_PUSH_DOWN);
	keybuf->enQueueKey(Bkey_lf | KEY_PUSH_UP);
}

void QTERuputer::pushPaletteRight(void)
{
	keybuf->enQueueKey(Bkey_rg | KEY_PUSH_DOWN);
	keybuf->enQueueKey(Bkey_rg | KEY_PUSH_UP);
}



void QTERuputer::openFile(void)
{
	pthread_mutex_lock(&eventmutex);
	event_return_value = 1;

	QTEFileDialog filedialog(this);

	if( filedialog.exec() == QDialog::Accepted )
	{
		Log::getInstance()->output(Log::Info, "Load file (%s)\n", filedialog.getSelectFileName());

		requestNewLoad( filedialog.getSelectFileName() );
	}

	frame->sync();

	pthread_mutex_unlock(&eventmutex);
}

// 以下 Qt/E のevent関連
void QTERuputer::paintEvent(QPaintEvent *e)
{
#if 0
	qtelcd->updateWindow();
	fprintf(stderr, "foo\n");
	fflush(stderr);
#endif
	resize(qtelcd->getWindowWidth(), qtelcd->getWindowHeight());
}

#if 0
void QTERuputer::moveEvent(QMoveEvent *e)
{
	pthread_mutex_lock(&eventmutex);
	event_return_value = 1;
	QWidget::moveEvent(e);
	pthread_mutex_unlock(&eventmutex);
}

void QTERuputer::resizeEvent(QResizeEvent *e)
{
	pthread_mutex_lock(&eventmutex);
	event_return_value = 1;
	QWidget::resizeEvent(e);
	pthread_mutex_unlock(&eventmutex);
}
#endif

void QTERuputer::closeEvent(QCloseEvent *e)
{
	pthread_mutex_lock(&eventmutex);
	event_return_value = 1;
//	getState()->setState(STATE_CLOSE);
	requestDestroy();
//	QWidget::closeEvent(e);
	e->accept();
	pthread_mutex_unlock(&eventmutex);

}



void QTERuputer::keyPressEvent(QKeyEvent *e)
{
	switch(e->key())
	{
	case Key_Up:
		keybuf->enQueueKey(Bkey_up | KEY_PUSH_DOWN);
		e->accept();
		break;
	case Key_Down:
		keybuf->enQueueKey(Bkey_dw | KEY_PUSH_DOWN);
		e->accept();
		break;
	case Key_Left:
		keybuf->enQueueKey(Bkey_lf | KEY_PUSH_DOWN);
		e->accept();
		break;
	case Key_Right:
		keybuf->enQueueKey(Bkey_rg | KEY_PUSH_DOWN);
		e->accept();
		break;
	case Key_W:
		keybuf->enQueueKey(Bkey_B | KEY_PUSH_DOWN);
		e->accept();
		break;
	case Key_S:
		keybuf->enQueueKey(Bkey_C | KEY_PUSH_DOWN);
		e->accept();
		break;
	case Key_X:
		keybuf->enQueueKey(Bkey_D | KEY_PUSH_DOWN);
		e->accept();
		break;
	case Key_Z:
		keybuf->enQueueKey(Bkey_A | KEY_PUSH_DOWN);
		e->accept();
		break;
	}
}

void QTERuputer::keyReleaseEvent(QKeyEvent *e)
{
	switch(e->key())
	{
	case Key_Up:
		keybuf->enQueueKey(Bkey_up | KEY_PUSH_UP);
		e->accept();
		break;
	case Key_Down:
		keybuf->enQueueKey(Bkey_dw | KEY_PUSH_UP);
		e->accept();
		break;
	case Key_Left:
		keybuf->enQueueKey(Bkey_lf | KEY_PUSH_UP);
		e->accept();
		break;
	case Key_Right:
		keybuf->enQueueKey(Bkey_rg | KEY_PUSH_UP);
		e->accept();
		break;
	case Key_W:
		keybuf->enQueueKey(Bkey_B | KEY_PUSH_UP);
		e->accept();
		break;
	case Key_S:
		keybuf->enQueueKey(Bkey_C | KEY_PUSH_UP);
		e->accept();
		break;
	case Key_X:
		keybuf->enQueueKey(Bkey_D | KEY_PUSH_UP);
		e->accept();
		break;
	case Key_Z:
		keybuf->enQueueKey(Bkey_A | KEY_PUSH_UP);
		e->accept();
		break;
	}
}

SDFindFile *QTERuputer::createSDFindFile(struct dta_t *dta_area, char const *pathname, unsigned char search_attribute)
{
	return new UnixFindFile(dta_area, pathname, search_attribute);
}

void *QTERuputer::thread_func(void *_qteruputer)
{
	QTERuputer *qteruputer = (QTERuputer *)_qteruputer;
	qteruputer->main_loop();
	return 0;
}
