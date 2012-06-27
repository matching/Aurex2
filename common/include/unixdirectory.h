// -*-c++-*-
// $Id: unixdirectory.h,v 1.8 2006/01/05 09:45:06 fumi Exp $

#ifndef UNIXDIRECTORY_H
#define UNIXDIRECTORY_H


#include "directory.h"

class UnixDirectory : public Directory
{
  public:
	UnixDirectory(void);
	UnixDirectory(char const *path);

	virtual int chdir(char const *path);
	int getdir(char *path);
	void setdir(char const *path);

	int getPathDelimiter(void);
	int getRestPath(char const *src, char *restpath) throw(DirectoryException);
	virtual void getReplacePath(char *dst);
	bool isAbsoluteDirectory(char const *path);

	void caseConvertion(char *path);

	virtual Directory *createDirectory(DirStack *root);
	virtual bool isSetupDirectory(void);
};

#endif
