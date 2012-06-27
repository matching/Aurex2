// -*-c++-*-
// $Id: winlogoutputevent.h,v 1.1 2006/01/26 18:42:45 fumi Exp $
#ifndef WINLOGOUTPUTEVENT_H
#define WINLOGOUTPUTEVENT_H

#include <windows.h>

#include "logoutput.h"


class WINLogOutputEvent : public LogOutput
{
	HANDLE hEventSrc;

  public:
	WINLogOutputEvent(void);
	~WINLogOutputEvent(void);

	void output(char const *buf);
};

#endif
