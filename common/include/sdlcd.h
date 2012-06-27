//-*-mode:c++; coding:euc-jp-unix-*-
// $Id: sdlcd.h,v 1.12 2005/01/13 08:40:42 fumi Exp $

#ifndef SDLCD_H
#define SDLCD_H

class CMLcd;
class SDLcdFont;

/*
 * system depend
 * システム依存のー
 * 継承して使用
 */

class SDLcd
{
  protected:
	CMLcd *cmlcd;

  public:
	SDLcd(CMLcd *_cmlcd);
	virtual ~SDLcd(void);

	virtual bool hasFontAa(void);
	virtual void updateWindow(void)=0;
	virtual SDLcdFont *createSDLcdFont(int width, int height, int prop, int gap, bool latin)=0;
	virtual SDLcdFont *createSDLcdFontAa(int width, int height, int prop, int gap, bool latin);
};




#endif

