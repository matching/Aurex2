// -*-c++-*-
// $Id: curlogoutput.h,v 1.1 2006/02/19 13:04:40 fumi Exp $

#ifndef CURLOGOUTPUT_H
#define CURLOGOUTPUT_H

#include <stdio.h>
#include "logoutput.h"

class CURLogOutput : public LogOutput
{
  private:
	int bottom;

  public:
	CURLogOutput( int bottom);
	~CURLogOutput(void);

	void output(char const *buf);
};

#endif
