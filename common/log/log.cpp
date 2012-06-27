/*
 * log.cpp - Log class
 *
 * $Id: log.cpp,v 1.3 2006/02/09 17:58:38 fumi Exp $
 *
 * Copyright (C) 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
 * All rights reserved.
 *
 * This is free software with ABSOLUTELY NO WARRANTY.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA
 */


#include <stdio.h>
#include <stdarg.h>

#include "log.h"
#include "logoutput.h"

Log *Log::instance = NULL;

Log::Log(void)
{
	logOutput = NULL;
	logLevel = Log::Info;
}

Log::~Log(void)
{

}

Log *Log::getInstance(void)
{
	if( instance == NULL )
		instance = new Log();

	return instance;
}

void Log::output(LogLevel logLevel, char const *format, ...)
{
/* エラー時はむりやりに標準エラーに出力できるようにする */
	if( logLevel != Log::Error && ( this->logLevel > logLevel || logOutput == NULL ) )
		return;

	va_list args;
	static char str[1024];

	va_start(args, format);

	vsnprintf(str, sizeof(str), format, args);

	if( logOutput )
		logOutput->output( str );
	else
	{
		fputs( str, stderr );
		fflush(stderr);
	}

	va_end(args);
}


void Log::output2(LogLevel logLevel, char const *buf)
{
/* エラー時はむりやりに標準エラーに出力できるようにする */
	if( logLevel != Log::Error && ( this->logLevel > logLevel || logOutput == NULL ) )
		return;

	if( logOutput )
		logOutput->output( buf );
	else
	{
		fputs( buf, stderr );
		fflush(stderr);
	}
}

void Log::setLogOutput(LogOutput *logOutput)
{
	freeLogOutput();
	this->logOutput = logOutput;
}

void Log::freeLogOutput(void)
{
	delete logOutput;
	logOutput = NULL;
}

void Log::setLogLevel(LogLevel val)
{
	this->logLevel = val;
}

