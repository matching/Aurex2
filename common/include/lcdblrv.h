// -*-c++-*-
// $Id: lcdblrv.h,v 1.8 2005/01/06 14:21:33 fumi Exp $

#ifndef LCDBLRV_H
#define LCDBLRV_H

#include <stdlib.h>
#include <list>
using namespace std;

class CMLcd;

enum{
	BLINK_REVERSE_START,
	BLINK_REVERSE_STOP,
};

enum BlinkReversePixelValue
{
	OFF,
	ON,
	REVERSE,
};

class LcdBlRv
{
  protected:
	int x, y;
	int width;
	int height;

	int state;

	enum BlinkReversePixelValue pixel_state;

  public:
	LcdBlRv(int x1, int y1, int x2, int y2);

	enum BlinkReversePixelValue const *getPixelSatatePointer(void);

	virtual int proceed(void) = 0;
	int getState(void);
	void setState(int s);


	int getX(void);
	int getY(void);
	int getWidth(void);
	int getHeight(void);

  protected:
	void off(void);
	void invert(void);

  public:
	void pixel_on(void);
};

#endif

