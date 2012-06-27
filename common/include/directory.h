// -*-c++-*-
// $Id: directory.h,v 1.18 2006/01/05 09:45:05 fumi Exp $

#ifndef DIRECTORY_H
#define DIRECTORY_H

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#include "codingsystem.h"

#include "throwable.h"
class DirectoryException : public Throwable
{
};

class DirectoryUndefinedExcption : public DirectoryException
{
};


class DirStack;
class CodingBase;

class Directory
{
  protected:
	DirStack *dirstack;

	enum CodingSystem coding;

  public:
	Directory(void);
	virtual ~Directory(void);

	virtual int chdir(char const *path)=0;
	virtual int getdir(char *path)=0;
	virtual void setdir(char const *path) = 0;

	virtual int getPathDelimiter(void)=0;
	virtual int getRestPath(char const *src, char *restpath)=0;
	virtual void getReplacePath(char *dst)=0;
	virtual bool isAbsoluteDirectory(char const *path)=0;

	virtual void caseConvertion(char *path)=0;

	virtual bool isSetupDirectory(void);

	virtual bool isExistentDirectory(void);

	virtual void splitDirFile(char const *src, char *dir, char *file);
#if 0
	void getFullFileName(char const *filename, char *fullFileName) throw(DirectoryUndefinedExcption);
#endif

	virtual Directory *createDirectory(DirStack *root) = 0;

	enum CodingSystem getCodingSystem(void);
	CodingBase *createCoding(char const *str);
};


#endif
