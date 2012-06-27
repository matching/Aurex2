// -*-c++-*-
// $Id: x11lcdfont.h,v 1.7 2006/02/11 19:17:23 fumi Exp $

#ifndef X11LCDFONT_H
#define X11LCDFONT_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "sdlcdfont.h"

class X11FontSet;
class CesSjis;

class X11LcdFont : public SDLcdFont
{
  protected:
	Display *display;
	Window window;
	GC gc;
	Pixmap pixmap;

	XFontSet fontset;
	int width, height;
	int gap;
	int ascent;

	unsigned char **buffer;
	unsigned long black, white;

	int maxWidth;
	int maxHeight;


  public:
	X11LcdFont(Display *_display, Window _window, int _width, int _height, int ascent, int gap, XFontSet fontset, int maxWidth, int maxHeight);
	virtual ~X11LcdFont(void);

	virtual unsigned char **drawString(char const *str, int *resultwidth);
	void freeBuffer(unsigned char **buf);
};


class X11LcdFontSjis : public X11LcdFont
{
	CesSjis *sjis;

  public:
	X11LcdFontSjis(Display *_display, Window _window, int _width, int _height, int ascent, int gap, XFontSet fontset, int maxWidth, int maxHeight);
	~X11LcdFontSjis(void);
	unsigned char **drawString(char const *str, int *resultwidth);


};

#endif
