// -*-c++-*-
// $Id: fdmgr.h,v 1.5 2004/04/29 09:44:32 fumi Exp $

#ifndef FDMGR_H
#define FDMGR_H

#include <stdio.h>

#include <map>
using namespace std;


// singleton
class FDMgr
{
  public:
	static FDMgr *getInstance(void);

  private:
	static FDMgr *instance;
	static unsigned short fdno;

	map<unsigned short, FILE *> fdmap;

	FDMgr(void);
	~FDMgr(void);

  public:
	unsigned short addFD(FILE *fp);
	FILE *getFP(unsigned short fd);

	unsigned short getFD(FILE *fp);

	void delFD(unsigned short fd);
};


#endif
