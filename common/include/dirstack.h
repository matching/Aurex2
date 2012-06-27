// -*-c++-*-
// $Id: dirstack.h,v 1.6 2005/01/06 14:21:33 fumi Exp $

#ifndef DIRSTACK_H
#define DIRSTACK_H

#include "directoryelement.h"

class DirStack
{
  protected:
	DirectoryElement head;

  public:
	DirStack(void);
	DirStack(char const *dir, char dirsep);
	virtual ~DirStack(void);

  protected:
	void makeDirStack(char const *dir, char dirsep);
	void push(char const *start, char const *end);
	void push(char const *dir);
	void push(DirectoryElement *de);
	void print(void);

  public:
	bool isSetup(void);
	void concat(DirStack *p);
	void normalize(void);
	void toFullPath(char *dst, char dirsep);
	DirStack *rest(DirStack *dst);

#if 0
	DirStack *clone(void);
#endif

};

#endif
