// -*-c++-*-
// $Id: cmlcd.h,v 1.17 2005/01/06 14:21:33 fumi Exp $

#ifndef CMLCD_H
#define CMLCD_H

class Ruputer;
class BlinkMgr;
class ReverseMgr;

class CMLcdFunc;

enum UpdateRegion {
	ALL,
	PART};
/*
 */
class CMLcd
{
  private:
	bool latin;
	Ruputer *ruputer;

	int expansion;

	enum UpdateRegion updateRegion;

	CMLcdFunc *func;

  public:
	BlinkMgr *blinkmgr;
	ReverseMgr *reversemgr;

	void updateAllPixel(void);
	void updatePartPixel(void);

	int getExpansion(void);
	void setExpansion(int expansion);

	int getRealWidth102(void);
	int getRealWidth320(void);
	int getRealHeight64(void);
	int getRealHeight240(void);

	CMLcd(Ruputer *ruputer);
	~CMLcd(void);

	void updatePixel(void);
	CMLcdFunc *getCMLcdFunc(void);
	CMLcdFunc *createCMLcdFunc(int expansion);
};

#endif
