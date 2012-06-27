//-*-c++-*-
// $Id: qteruputer.h,v 1.10 2006/02/19 03:01:35 fumi Exp $

#ifndef QTERUPUTER_H
#define QTERUPUTER_H

#include "ruputer.h"
#include <qwidget.h>
#include <qmainwindow.h>
#include <qpopupmenu.h>

class QPaintEvent;
class QKeyEvent;
class QResizeEvent;
class QMoveEvent;
class QToolBar;
class QAction;

class Option;
class UFilerIni;

class QTELcd;
class QTERuputer : public QMainWindow, public Ruputer
{
  Q_OBJECT

  private:
	QTELcd *qtelcd;
	QAction *openAction;
	QAction *paletteAction;

	QPopupMenu *openFileMenu;

	QToolBar *palette;
	QAction  *enterAction;
	QAction  *elAction;
	QAction  *menuAction;
	QAction  *filerAction;
	QAction  *upAction;
	QAction  *downAction;
	QAction  *rightAction;
	QAction  *leftAction;

	int event_return_value;


  public:
	QTERuputer(Frame *_frame, Directory *_sddir, Option *option, UFilerIni *ini);
	~QTERuputer(void);

	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);
#if 0
	void moveEvent(QMoveEvent *e);
	void resizeEvent(QResizeEvent *e);
#endif

	void closeEvent(QCloseEvent *e);


	void paintEvent(QPaintEvent *e);
	static void *thread_func(void *arg);

	int load(char const *filename) throw();

	SDFindFile *createSDFindFile(struct dta_t *dta_area, char const *pathname, unsigned char search_attribute);

  private:
	int EventProcedure(void);

  public slots:
	void openFile(void);

	void pushPaletteEnter(void);
	void pushPaletteMenu(void);
	void pushPaletteEl(void);
	void pushPaletteFiler(void);
	void pushPaletteUp(void);
	void pushPaletteDown(void);
	void pushPaletteLeft(void);
	void pushPaletteRight(void);


	void pauseEmulate(void);
	void resumeEmulate(void);
};


#endif

