/* -*-c++-*- */

#ifndef UNIXFRAME_H
#define UNIXFRAME_H

#include "frame.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

class UnixFrame : public Frame
{
  private:
	unsigned long sec;

  public:
	UnixFrame(void) : Frame()
	{
		struct timeval tp;

		gettimeofday(&tp, NULL);

		sec=tp.tv_sec;
	}

  private:
	unsigned long getMilliSec(void)
	{
		struct timeval tp;

		gettimeofday(&tp, NULL);

		return (unsigned long)(tp.tv_sec-sec)*1000+tp.tv_usec/1000;
	}

	void waitMilliSec(unsigned long us)
	{
		usleep(us);
	}
};



#endif
