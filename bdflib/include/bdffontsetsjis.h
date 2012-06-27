// -*-c++-*-
/* $Id: bdffontsetsjis.h,v 1.3 2005/01/24 03:49:07 fumi Exp $
 */

#ifndef BDFFONTSETSJIS_H
#define BDFFONTSETSJIS_H

#include "bdffontsets.h"

class BdfFont;
class BdfFontSet;

/** JIS code のフォントを使用する
 */
class BdfFontSetsJis : public BdfFontSets{
  private:
	BdfFontSet *jisx0201;
	BdfFontSet *jisx0208;

  public:
	BdfFontSetsJis(BdfFontSet *jisx0201, BdfFontSet *jisx0208);
	virtual ~BdfFontSetsJis(void);

	BdfFont *getFontData(unsigned short code);
};

#endif
