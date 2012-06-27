// -*-c++-*-
/* $Id: bdffontsets.h,v 1.3 2004/05/07 08:25:03 fumi Exp $
 */

#ifndef BDFFONTSETS_H
#define BDFFONTSETS_H

class BdfFont;

/** 複数の BDF ファイルを管理するクラス
 * 継承して使用する
 */
class BdfFontSets{
  public:
	BdfFontSets(void);
	virtual ~BdfFontSets(void);

	virtual BdfFont *getFontData(unsigned short code)=0;
};

#endif
