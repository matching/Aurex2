// -*-c++-*-
// $Id: filerinimgr.h,v 1.6 2005/01/06 14:21:33 fumi Exp $

#ifndef FILERINIMGR_H
#define FILERINIMGR_H

class FilerIni;
class UFilerIni;
class Directory;
class FilerIniBase;

class FilerIniMgr
{
  private:
	FilerIni *filer;
	UFilerIni *ufiler;

  public:
	FilerIniMgr(FilerIni *fini, UFilerIni *uini);
	~FilerIniMgr(void);

#if 0
	int loadFilerIni(Directory *rootdir);
	int loadUFilerIni(Directory *dir, char const *filename);
#endif
	char const *getExecutableFileName(char const *filename);

	void getExtList(char *out);
};

#endif
