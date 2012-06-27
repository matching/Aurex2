// -*-c++-*-
// $Id: curlogoutput.h,v 1.1 2006/02/19 13:04:40 fumi Exp $

#ifndef ANDROIDLOGOUTPUT_H
#define ANDROIDLOGOUTPUT_H

#include <android/log.h>
#include "logoutput.h"

class AndroidLogOutput : public LogOutput
{
  private:

  public:
	AndroidLogOutput(void);
	~AndroidLogOutput(void);

	void output(char const *buf);
};

#endif
