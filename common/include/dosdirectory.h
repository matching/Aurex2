// -*-c++-*-
// $Id: dosdirectory.h,v 1.10 2006/01/05 09:45:05 fumi Exp $

#ifndef DOSDIRECTORY_H
#define DOSDIRECTORY_H

#include "directory.h"

class DosDirectory : public Directory
{
  protected:
	char drive;

  public:
	DosDirectory(void);
	DosDirectory(char const *path);

	virtual int chdir(char const *path);
	int getdir(char *path);

	void setdir(char const *path);

	int getPathDelimiter(void);
	bool isAbsoluteDir(char const *path);
	virtual int getRestPath(char const *src, char *restpath) throw(DirectoryException);
	virtual void getReplacePath(char *dst);
	bool isAbsoluteDirectory(char const *path);
	void caseConvertion(char *path);

	virtual bool isSetupDirectory(void);
	virtual Directory *createDirectory(DirStack *root);

	void splitDirFile(char const *src, char *dir, char *file);

  protected:
	bool isSameDriveLetter(char dst_drive);
};

#endif

