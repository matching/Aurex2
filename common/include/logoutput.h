// -*-c++-*-
// $Id: logoutput.h,v 1.1 2006/01/05 09:45:05 fumi Exp $

#ifndef LOGOUTPUT_H
#define LOGOUTPUT_H

#include <stdio.h>

class LogOutput
{
  public:
	LogOutput(void);
	virtual ~LogOutput(void);

	virtual void output(char const *buf) = 0;
};

#endif
