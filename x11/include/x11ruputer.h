//-*-c++-*-
// $Id: x11ruputer.h,v 1.6 2005/01/08 15:51:53 fumi Exp $

#ifndef X11RUPUTER_H
#define X11RUPUTER_H

#include "ruputer.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>


class X11Lcd;
class X11FontSet;
class X11Ruputer : public Ruputer
{
  private:
	X11Lcd *x11lcd;
	X11FontSet *x11fontset;

	Window window;
	Display *display;

  public:
	X11Ruputer(int argc, char **argv, Frame *_frame, Directory *_sddir, Option *option, UFilerIni *uini);
	~X11Ruputer(void);

	int load(char const *filename) throw();

	SDFindFile *createSDFindFile(struct dta_t *dta_area, char const *pathname, unsigned char search_attribute);

  private:

	int EventProcedure(void);
};


#endif

