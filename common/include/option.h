// -*-c++-*-
// $Id: option.h,v 1.19 2006/01/22 15:35:28 fumi Exp $

#ifndef OPTION_H
#define OPTION_H


#include <string>
#include <list>
#include <map>
using namespace std;

#include "optionlist.h"



#define OPT_WARN_ODD_ACCESS "Common.Misc.WarnOddAccess"
#define OPT_SOUND           "Common.Misc.Sound"
#define OPT_CPU_TIMER       "Common.Misc.CpuTimer"
#define OPT_LOG             "Common.Misc.Log"
#define OPT_CPU_SPEED       "Common.Speed.CpuSpeed"
#define OPT_TIMER_SPEED     "Common.Speed.TimerSpeed"
#define OPT_ROOT_DIRECTORY  "Common.Directory.RootDirectory"

#define OPT_AFTER_IMAGE     "Common.LCD.AfterImage"
#define OPT_ANTI_ALIAS      "Common.LCD.Antialias"
#define OPT_EXPANSION       "Common.LCD.Expansion"
#define OPT_AI_VALUE        "Common.LCD.AfterImageTime"

#define OPT_USE_ISO8859_1   "Common.onHand.useIso8859_1"

#define OPT_LOG_LEVEL       "Common.Log.Level"
#define OPT_LOG_TYPE        "Common.Log.Type"
#define OPT_LOG_FILE        "Common.Log.File"


class OptionList;
class OptionBool;
class OptionInt;
class OptionString;

class Option
{
  private:
	OptionBool *warn_odd_access;
	OptionBool *enable_sound;
	OptionBool *enable_cpu_timer;
// 	bool enable_rom_font; µÒ»›§π§Îπ‡Ã‹§¨§ §§
	OptionInt *cpu_speed;
	OptionInt *timer_speed;
//	OptionBool *enable_gray;
	OptionString *root_directory;

	OptionBool *enable_after_image;
	OptionBool *enable_anti_alias;
	OptionInt *expansion;

	OptionInt *ai_value;

	OptionBool *useIso8859_1;

	OptionInt *log_level;
	OptionInt *log_type;
	OptionString *log_file;

	OptionList optionlist;
	list<OptionItem *> optionitems;

  public:
	Option(void);
	~Option(void);

	void addOptionItem(OptionItem *item);

	bool load(char const *filename);
	bool save(char const *filename);

	bool get_warn_odd_access(void);
	void set_warn_odd_access(bool v);

	bool get_enable_sound(void);
	void set_enable_sound(bool v);

	bool get_enable_cpu_timer(void);
	void set_enable_cpu_timer(bool v);

	int  get_cpu_speed(void);
	void set_cpu_speed(int v);

	int  get_timer_speed(void);
	void set_timer_speed(int v);

	char const *get_root_directory(void);
	void        set_root_directory(char const *v);

	bool get_after_image(void);
	void set_after_image(bool v);

	bool get_anti_alias(void);
	void set_anti_alias(bool v);

	int get_expansion(void);
	void set_expansion(int v);

	int get_ai_value(void);
	void set_ai_value(int v);

	bool get_useIso8859_1(void);
	void set_useIso8859_1(bool v);


	int  get_log_level(void);
	void set_log_level(int v);

	int  get_log_type(void);
	void set_log_type(int v);

	char const *get_log_file(void);
	void        set_log_file(char const *v);

	void syncToOptionList(void);

  private:
	void syncFromOptionList(void);

//	void syncToValue(void);
};

extern Option *option;

#endif
