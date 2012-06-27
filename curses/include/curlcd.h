//-*-c++-*-
#ifndef CURLCD_H
#define CURLCD_H

#include <curses.h>
#include "sdlcd.h"

class BdfMgr;

class CURLcd : public SDLcd
{
  private:
	BdfMgr *bdfmgr;

	unsigned char pixelChars[ 256 ];

  public:
	CURLcd(CMLcd *cmlcd);
	~CURLcd(void);
	int getBottomLine(void);


	virtual void updateWindow(void);

	SDLcdFont *createSDLcdFont(int width, int height, int prop, int gap, bool latin);
};

#endif
