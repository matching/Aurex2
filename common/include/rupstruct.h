// -*-c++-*-
// $Id: rupstruct.h,v 1.5 2004/04/29 09:44:32 fumi Exp $

#ifndef RUPSTRUCT_H
#define RUPSTRUCT_H

class MemoryForInside;

class WatchDT
{
  private:
	MemoryForInside *mem;
	struct tm *local;

  public:
	WatchDT(void);
	~WatchDT(void);

	int getYear(void);
	int getMonth(void);
	int getWDay(void);
	int getMDay(void);
	int getHour(void);
	int getMin(void);
	int getSec(void);

	int getYearBcd(void);
	int getMonthBcd(void);
	int getMDayBcd(void);
	int getHourBcd(void);
	int getMinBcd(void);
	int getSecBcd(void);

	int toBcd(int num);

	void updateWatchDT(void);
};

#endif
