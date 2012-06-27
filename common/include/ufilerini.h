// -*-c++-*-
// $Id: ufilerini.h,v 1.4 2005/07/27 18:53:28 fumi Exp $

#ifndef UFILERINI_H
#define UFILERINI_H

#include "filerinibase.h"

class UFilerIni : public FilerIniBase
{
  public:
	UFilerIni(void);
	virtual ~UFilerIni(void);

	void load(char const *filename);
};

#endif

