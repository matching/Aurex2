// -*-c++-*-
// $Id: bdffontset.h,v 1.3 2004/05/07 08:25:03 fumi Exp $

#ifndef BDFFONTSET_H
#define BDFFONTSET_H

#include <map>
using namespace std;

class BdfFont;

/** ひとつのBDFファイルに相当するクラス
 *
 */
class BdfFontSet{
  private:
    // (code . data)
	map<unsigned short, BdfFont *> data;
	int height;

  public:
	BdfFontSet(void);
	virtual ~BdfFontSet(void);

	int parse(char const *filename, int height);

	void add(unsigned short code, int width, unsigned char const *data, int data_len);
	BdfFont *getBdfFont(unsigned short code);
};

#endif
