/*
 * winpropwindow.cpp - Windows settings window
 *
 * $Id: winpropwindowlog.cpp,v 1.1 2006/01/26 18:42:46 fumi Exp $
 *
 * Copyright (C) 2004, 2005, 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

#include <windows.h>
#include <windowsx.h>

#include <assert.h>

#include "winresource.h"
#include "winpropwindowlog.h"

WNDPROC WINPropWindowLog::hRadioProc[4] = {
	radioButtonProc_none,
	radioButtonProc_stderr,
	radioButtonProc_file,
	radioButtonProc_event,
};

static WINPropWindowLog *gWINPropWindowLog;

WINPropWindowLog::WINPropWindowLog(HWND hWnd)
{
	gWINPropWindowLog = this;

	this->hWnd = hWnd;

	hRadioWnd[0] = GetDlgItem(hWnd, IDC_RADIO_LOG_NONE);
	hRadioWnd[1] = GetDlgItem(hWnd, IDC_RADIO_LOG_STDERR);
	hRadioWnd[2] = GetDlgItem(hWnd, IDC_RADIO_LOG_FILE);
	hRadioWnd[3] = GetDlgItem(hWnd, IDC_RADIO_LOG_EVENT);

	hLogFileWnd = GetDlgItem(hWnd, IDET_LOG_FILE_NAME);
	hLogFilePushButtonWnd = GetDlgItem(hWnd, IDPB_LOG_FILE_SELECT);
}

WINPropWindowLog::~WINPropWindowLog(void)
{
	gWINPropWindowLog = NULL;
}

void WINPropWindowLog::enSubclass(void)
{
	int i;
	for(i = 0; i < 4; i++)
	{
		hOrigRadioProc[i] = (WNDPROC)GetWindowLong( hRadioWnd[i], GWL_WNDPROC );

		SetWindowLong( hRadioWnd[i], GWL_WNDPROC, (LONG)hRadioProc[i]);
	}
}

void WINPropWindowLog::deSubclass(void)
{
	int i;
	for(i = 0; i < 4; i++)
	{
		SetWindowLong( hRadioWnd[i], GWL_WNDPROC, (LONG)hOrigRadioProc[i] );
	}
}

WNDPROC WINPropWindowLog::getOrigRagioProc( HWND hWnd )
{
	int i;
	for(i = 0; i < 4; i++)
	{
		if( hWnd == hRadioWnd[i] )
			return hOrigRadioProc[i];
	}

	assert( 0 );

	return NULL;
}

void WINPropWindowLog::disableLogFile(void)
{
	Edit_SetReadOnly( hLogFileWnd, true );
	Button_Enable( hLogFilePushButtonWnd, false);
}

void WINPropWindowLog::enableLogFile(void)
{
	Edit_SetReadOnly( hLogFileWnd, false );
	Button_Enable( hLogFilePushButtonWnd, true);
}

LRESULT CALLBACK WINPropWindowLog::radioButtonProc_none(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch( msg )
	{
	case WM_KEYDOWN:
		if( wp != VK_SPACE )
			break;
// fall through
	case WM_LBUTTONDOWN:
		gWINPropWindowLog->disableLogFile();
// いずれにしても、デフォルトのプロシージャ呼ぶ
	}

	return CallWindowProc( gWINPropWindowLog->getOrigRagioProc( hWnd ) , hWnd, msg, wp, lp );
}

LRESULT CALLBACK WINPropWindowLog::radioButtonProc_event(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	return WINPropWindowLog::radioButtonProc_none(hWnd, msg, wp, lp);
}

LRESULT CALLBACK WINPropWindowLog::radioButtonProc_stderr(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	return WINPropWindowLog::radioButtonProc_none(hWnd, msg, wp, lp);
}

LRESULT CALLBACK WINPropWindowLog::radioButtonProc_file(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch( msg )
	{
	case WM_KEYDOWN:
		if( wp != VK_SPACE )
			break;
// fall through
	case WM_LBUTTONDOWN:
		gWINPropWindowLog->enableLogFile();
// いずれにしても、デフォルトのプロシージャ呼ぶ
	}

	return CallWindowProc( gWINPropWindowLog->getOrigRagioProc( hWnd ) , hWnd, msg, wp, lp );
}


