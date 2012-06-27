// -*-c++-*-
// $Id: logoutputfile.h,v 1.1 2006/01/05 09:45:05 fumi Exp $

#ifndef LOGOUTPUTFILE_H
#define LOGOUTPUTFILE_H

#include <stdio.h>
#include "logoutput.h"

class LogOutputFile : public LogOutput
{
  private:
	FILE *fp;

  public:
	LogOutputFile(char const *filename);
	~LogOutputFile(void);

	void output(char const *buf);
};

#endif
