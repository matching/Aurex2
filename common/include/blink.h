// -*-c++-*-
// $Id: blink.h,v 1.7 2005/01/06 14:21:33 fumi Exp $

#ifndef BLINK_H
#define BLINK_H

#include "lcdblrv.h"

class Blink : public LcdBlRv
{
  private:
	int on;		/* on = 1, off = 0 */
	int type;	/* 0:off, 1:invert, 2:on/off				*/
	int hz;		/* 0:1.5/0.5, 1:0.5/0.5, 2:0.25/0.25 3:私が使う	*/
	int count;	/* カウント(Ruputer 側のデータではない) 		*/

	static char blink_count_reset[4];

  public:
	Blink(int x1, int y1, int x2, int y2, int color);
	int proceed(void);
};

#endif
