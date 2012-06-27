/* -*-c++-*- */
// $Id: lcd.h,v 1.7 2004/10/31 13:41:17 fumi Exp $

#ifndef LCD_H
#define LCD_H

class CMLcd;
class Ruputer;
class RupAddress;

class LcdIntr
{
  private:

// System Depend ...
	CMLcd *cmlcd;
	RupAddress *systemAddress;

  public:
	LcdIntr(Ruputer *ruputer);

	void callLcdFunc(Register *regs);

  private:

	long screen(void);
	long cls(void);
	long getbufsize(void);
	long lcdfreeze(void);
	long getscreen(void);
	long lcdon(void);
	long getlcdon(void);
	long mojisize(void);
	long reverse(void);
	long revstop(void);
	long revstart(void);
	long revend(void);
	long revdraw(void);
	long blend(void);
	long blstop(void);
	long blink(void);
	long blstart(void);
	long gv_place(void);
	long gv_kput(void);
	long gv_line(void);
	long gv_square(void);
	long gv_pset(void);
	long gv_point(void);
	long gv_get(void);
	long gv_put(void);
	long gv_reverse(void);
	long gv_clear(void);
	long gv_aput(void);
	long gv_circle(void);
	long gv_sput(void);
	long gv_scroll(void);
};

#endif
