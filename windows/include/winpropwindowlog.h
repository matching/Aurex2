// $Id: winpropwindowlog.h,v 1.1 2006/01/26 18:42:45 fumi Exp $

#ifndef WINPROPWINDOWLOG_H
#define WINPROPWINDOWLOG_H

#include <windows.h>


class WINPropWindowLog
{
  private:
	HWND hWnd;
	HWND hRadioWnd[4];
	WNDPROC hOrigRadioProc[4];

	HWND hLogFileWnd;
	HWND hLogFilePushButtonWnd;

	static WNDPROC hRadioProc[4];

  public:
	WINPropWindowLog(HWND hWnd);
	~WINPropWindowLog(void);

	void enSubclass(void);
	void deSubclass(void);

	void disableLogFile(void);
	void enableLogFile(void);

  private:

	WNDPROC getOrigRagioProc( HWND hWnd );

	static LRESULT CALLBACK radioButtonProc_none(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
	static LRESULT CALLBACK radioButtonProc_event(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
	static LRESULT CALLBACK radioButtonProc_stderr(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
	static LRESULT CALLBACK radioButtonProc_file(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
};


#endif

