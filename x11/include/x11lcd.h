//-*-c++-*-
// $Id: x11lcd.h,v 1.7 2005/01/08 15:51:53 fumi Exp $

#ifndef X11LCD_H
#define X11LCD_H

#include "sdlcd.h"
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

class X11FontSet;
class SDLcdFont;
class RupAddress;
class X11Lcd : public SDLcd
{
	Display *display;
	Window window;
	GC gc;
	XImage *ximage;

	X11FontSet *x11fontsets;

	int depth;

	RupAddress *systemAddress;
  public:
	X11Lcd(Display *display, Window window, CMLcd *cmlcd, X11FontSet *fontset, RupAddress *systemAddress);
	~X11Lcd(void);
	void updateWindow(void);

	SDLcdFont *createSDLcdFont(int width, int height, int prop, int gap, bool latin);
	SDLcdFont *createSDLcdFontAa(int width, int height, int prop, int gap, bool latin);
	bool hasFontAa(void);

};

#endif
