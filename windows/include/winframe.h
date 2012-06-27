/* -*-c++-*- */
// $Id: winframe.h,v 1.5 2004/04/29 14:12:51 fumi Exp $

#ifndef WINFRAME_H
#define WINFRAME_H

#include "frame.h"
#include <winbase.h>

class WINFrame : public Frame
{
  private:
	unsigned long getMilliSec(void)
	{
		return (unsigned long)GetTickCount();
	}

	void waitMilliSec(unsigned long us)
	{
		Sleep(us);
	}
};



#endif
