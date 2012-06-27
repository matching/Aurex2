//-*-c++-*-
// $Id: winpropwindow.h,v 1.5 2006/01/26 18:42:45 fumi Exp $
#ifndef WINPROPWINDOW_H
#define WINPROPWINDOW_H

#include <windows.h>

class WINPropWindow
{
  private:
	HINSTANCE hInstance;
	HWND hWnd;

	bool option_b_transparent;
	bool option_b_alpha;
	int  option_i_alpha_value;
	bool option_b_no_caption;
	bool option_b_statusbar;

	bool option_b_full_window;
	bool option_b_flip;
	int  option_i_expansion;
	bool option_b_antialiases;
	bool option_b_afterimage;

	char option_s_root_directory[ 1024 ];

	int  option_ai_value;

	int  option_log_type;
	char option_log_file[1024];

  public:
	WINPropWindow(HINSTANCE hInstance, HWND hWnd);
	~WINPropWindow(void);

	void show(void);

	void saveNowOption(void);
	bool isChanged_windowStyle(void);
	bool isChanged_winlcd(void);
	bool isChanged_cmlcd(void);
	bool isChanged_root_directory(void);
	bool isChanged_log(void);

  public:
	static LRESULT CALLBACK cbDialogMisc(HWND hWnd_dlg, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK cbDialogLcd(HWND hWnd_dlg, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK cbDialogSpeed(HWND hWnd_dlg, UINT msg, WPARAM wParam, LPARAM lParam);
//	static LRESULT CALLBACK cbDialogFont(HWND hWnd_dlg, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK cbDialogDirectory(HWND hWnd_dlg, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK cbDialogLog(HWND hWnd_dlg, UINT msg, WPARAM wParam, LPARAM lParam);

};

#endif
