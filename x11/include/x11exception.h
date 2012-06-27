//-*-c++-*-
// $Id: x11exception.h,v 1.2 2005/01/24 03:49:08 fumi Exp $

#ifndef X11EXCEPTION_H
#define X11EXCEPTION_H

#include "throwable.h"

class X11Exception : public Throwable
{
};

class X11DisplayOpenException : public X11Exception
{
};


#endif
