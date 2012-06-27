/* -*-c++-*- */
// $Id: sdfindfile.h,v 1.3 2006/02/22 17:40:39 fumi Exp $

#ifndef SDFINDFILE_H
#define SDFINDFILE_H

class SDFindFile;

struct dta_t{
	SDFindFile *sdff;
	char _recived[18];
/* 上 22 byte 予約領域 */

//	struct rup_time time __attribute__ ((packed));
//	struct rup_day day __attribute__ ((packed));
	unsigned char time[2]  __attribute__ ((__packed__));
	unsigned char date[2]  __attribute__ ((__packed__));
	unsigned long size __attribute__ ((__packed__));
	unsigned char attrib;
	char filename[13];
} __attribute__ ((__packed__)) ;
/* 44 byte */

/*
 * できれば、dta_area には継承クラスからはアクセスさせない方が良いような気がした
 */
class SDFindFile
{
  protected:
	struct dta_t *dta_area;
	char const *pathname;
	unsigned char search_attribute;

  public:
	SDFindFile(struct dta_t *dta_area, char const *pathname, unsigned char search_attribute);
	virtual ~SDFindFile();

	virtual int findFirst(void) = 0;
	virtual int findNext(void) = 0;

	void setDate(int year, int month, int day);
	void setTime(int hour, int min, int sec);
};


#endif

