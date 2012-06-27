//-*-c++-*-
#ifndef QTELCD_H
#define QTELCD_H

#include "sdlcd.h"
#include <stdio.h>

class QWidget;
class QColor;
class QPixmap;
class QDirectPainter;
class RupAddress;

#include <qwindowdefs.h>

class QTELcd : public SDLcd
{
  private:
	QWidget *parent;

	int width, height;
	int topHeight;

	int window_width, window_height;

	RupAddress *systemAddress;

  public:
	QTELcd(RupAddress *systemAddress, QWidget *_parent, CMLcd *cmlcd, int topHeight, int bottomHeight, int toolbarwidth);
	~QTELcd(void);
	void updateWindow(void);
	bool hasFontAa(void);

	int getWindowWidth(void);
	int getWindowHeight(void);

	SDLcdFont *createSDLcdFont(int width, int height, int prop, int gap, bool latin);
	SDLcdFont *createSDLcdFontAa(int width, int height, int prop, int gap, bool latin);

};

#endif
