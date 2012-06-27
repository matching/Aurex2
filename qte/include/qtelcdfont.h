// -*-c++-*-
// $Id: qtelcdfont.h,v 1.3 2005/07/27 13:50:54 fumi Exp $

#ifndef QTELCDFONT_H
#define QTELCDFONT_H

#include "sdlcdfont.h"
#include <qimage.h>

class CesSjis;
class QWidget;

class QTELcdFont : public SDLcdFont
{
  private:
	QImage image;

	CesSjis *sjis;
	QPixmap *pixmap;
	QWidget *parent;

	int width;
	int height;
	int gap;

	bool aaflag;

  public:
	QTELcdFont(QWidget *parent, int width, int height, int prop, int gap, int maxwidth, bool aaflag);
	~QTELcdFont(void);
	unsigned char **drawString(char const *str, int *resultwidth);
	void freeBuffer(unsigned char **buf);
};



#endif
