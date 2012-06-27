//-*-c++-*-
// $Id: winoption.h,v 1.12 2006/01/24 18:48:58 fumi Exp $

#ifndef WINOPTION_H
#define WINOPTION_H

#include "widgetoption.h"

class OptionBool;
class OptionInt;
class OptionString;

#define OPT_NO_CAPTION    "Windows.Misc.NoCaption"
#define OPT_ALPHA         "Windows.LCD.Alpha"
#define OPT_ALPHA_VALUE   "Windows.LCD.AlphaValue"
#define OPT_TRANSPARENT   "Windows.LCD.Transparent"
#define OPT_ALWAYS_ON_TOP "Windows.Misc.AlwaysOnTop"
#define OPT_X_POS         "Windows.Misc.XPos"
#define OPT_Y_POS         "Windows.Misc.YPos"
#define OPT_STATUS_BAR    "Windows.Misc.StatusBar"

#define OPT_ONLY_A_PROCESS "Windows.Misc.OnlyAProcess"

class WINOption : public WidgetOption
{
  private:
	OptionBool *no_caption;
	OptionBool *alpha;
	OptionInt *alpha_value;
	OptionBool *transparent;
	OptionBool *always_on_top;
	OptionInt *window_x_pos;
	OptionInt *window_y_pos;
	OptionBool *status_bar;
	OptionBool *only_a_process;

  public:
	WINOption(void);
	~WINOption(void);

	bool get_no_caption(void);
	void set_no_caption(bool v);

	bool get_alpha(void);
	void set_alpha(bool v);

	int get_alpha_value(void);
	void set_alpha_value(int v);

	bool get_transparent(void);
	void set_transparent(bool v);

	bool get_always_on_top(void);
	void set_always_on_top(bool v);


	int  get_window_x_pos(void);
	void set_window_x_pos(int v);

	int  get_window_y_pos(void);
	void set_window_y_pos(int v);

	bool get_status_bar(void);
	void set_status_bar(bool v);

	bool get_only_a_process(void);
	void set_only_a_process(bool v);

};

extern WINOption *winoption;

#endif
