// -*-c++-*-
// $Id: logoutputstderr.h,v 1.1 2006/01/05 09:45:05 fumi Exp $

#ifndef LOGOUTPUTSTDERR_H
#define LOGOUTPUTSTDERR_H

#include <stdio.h>
#include "logoutput.h"

class LogOutputStderr : public LogOutput
{
  public:
	LogOutputStderr(void);
	~LogOutputStderr(void);

	void output(char const *buf);
};

#endif
