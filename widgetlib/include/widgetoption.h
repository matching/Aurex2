// -*-c++-*-
// $Id: widgetoption.h,v 1.6 2006/01/07 17:59:14 fumi Exp $

#ifndef WIDGETOPTION_H
#define WIDGETOPTION_H

#define OPT_FULL_WINDOW "Wigdet.LCD.FullWindow"
#define OPT_FLIP        "Wigdet.LCD.Flip"

#include "option.h"
class OptionBool;

class WidgetOption : public Option
{
  private:
	OptionBool *full_window;
	OptionBool *flip;

  public:
	WidgetOption(void);
	virtual ~WidgetOption(void);

	bool get_full_window(void);
	void set_full_window(bool v);

	bool get_flip(void);
	void set_flip(bool v);
};

#endif
