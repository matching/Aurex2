// -*-c++-*-
// $Id: log.h,v 1.5 2006/02/19 13:04:39 fumi Exp $

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>

class LogOutput;

class Log
{
  public:
	enum LogLevel { CPUDebug = -2, Debug = -1, Info = 0, Warning = 1, Error = 2 };

	enum LogType { TYPE_NULL = 0, TYPE_STDERR = 1, TYPE_FILE = 2, TYPE_WINDOWS_EVENT = 3, TYPE_CURSES = 4 }; /* オプションで使用する */

  private:
	static Log *instance;
	LogOutput *logOutput;
	LogLevel logLevel;

  public:
	static Log *getInstance(void);

	void output(LogLevel logLevel, char const *format, ...);
	void output2(LogLevel logLevel, char const *buf);

	void setLogOutput(LogOutput *logOutput);
	void freeLogOutput(void);

	void setLogLevel(LogLevel val);

  private:
	Log(void);
	~Log(void);
};

#endif
