// -*-c++-*-
// $Id: lcdblrvmgr.h,v 1.9 2006/02/16 13:05:12 fumi Exp $

#ifndef LCDBLRVMGR_H
#define LCDBLRVMGR_H

#include <map>
using namespace std;

#include "lcdblrv.h"

class LcdBlRvMgr
{
  protected:
	static enum BlinkReversePixelValue const *pixel_status[240][320];

	map<int, LcdBlRv *> regions;
	int num;

  public:
	LcdBlRvMgr(void);
	virtual ~LcdBlRvMgr(void);

	int add(LcdBlRv *dat);
	int del(int no);
	int setState(int no, int state);
	int start(int no);
	int stop(int no);

	void closeAll(void);
	void proceed(void);

	inline static enum BlinkReversePixelValue getPixelStatus(int x, int y)
	{
		enum BlinkReversePixelValue const *p;

		p = pixel_status[y][x];

		if( p == NULL )
			return ON;
		return *p;
	}

  protected:
	int findFreeNo(void);

  private:
	void updatePixelStatus(LcdBlRv *dat, enum BlinkReversePixelValue const *p);

};

#endif
