// -*-c++-*-
// $Id: dirstacksjis.h,v 1.2 2005/01/06 14:21:33 fumi Exp $

#ifndef DIRSTACKSJIS_H
#define DIRSTACKSJIS_H

#include "dirstack.h"

class DirStackSjis : public DirStack
{
  public:
	DirStackSjis(void);
	DirStackSjis(char const *dir, char dirsep);
	virtual ~DirStackSjis(void);

  private:
	void makeDirStack(char const *dir, char dirsep);
};

#endif
