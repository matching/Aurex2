// -*-c++-*-
// $Id: bdfmgr.h,v 1.3 2004/05/07 08:25:03 fumi Exp $

#ifndef BDFMGR_H
#define BDFMGR_H

#include <map>
using namespace std;

class BdfFontSet;
class BdfFontSets;

/** BDF ファイル管理クラス
 *
 */
class BdfMgr{
  private:
// (height . fontset)
	map<int, BdfFontSet *> iso88591_fonts;
	map<int, BdfFontSet *> jisx0201_fonts;
	map<int, BdfFontSet *> jisx0208_fonts;

  public:
	BdfMgr(void);
	~BdfMgr(void);

	void add_iso88591_font(char const *fontname, int height);
	void add_jisx0201_font(char const *fontname, int height);
	void add_jisx0208_font(char const *fontname, int height);

	BdfFontSets *getJisBdfFont(int height);
	BdfFontSets *getIsoBdfFont(int height);

  private:
	BdfFontSet *createBdfFontSet(char const *filename, int height);
};

#endif
