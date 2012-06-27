//-*-c++-*-
#ifndef X11FONTSET_H
#define X11FONTSET_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <map>
using namespace std;

class X11FontSet
{
	map<unsigned long, XFontSet> fontsets;
	Display *display;

  public:
	X11FontSet(Display *d);
	~X11FontSet(void);
	XFontSet get(int height, int prop, bool latin);
};

#endif
