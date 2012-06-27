/*
 * winpropwindow.cpp - Windows settings window
 *
 * $Id: winpropwindow.cpp,v 1.7 2006/01/28 14:32:24 fumi Exp $
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <prsht.h>
#include <shlobj.h>

#include "winpropwindow.h"
#include "winpropwindowlog.h"
#include "winoption.h"
#include "winresource.h"

#include "log.h"

#define CHECK_WHEN_TRUE(a,b) {if( b ) Button_SetCheck(GetDlgItem(hWnd, a), BST_CHECKED);}
#define TRUE_WHEN_CHECKED(b) (Button_GetCheck(GetDlgItem(hWnd, b)) == BST_CHECKED ? true : false)


struct PropPage_t {
	char *temp;
	DLGPROC funcCallBack;
};

static PropPage_t props[] = {
	{"PROP_PAGE_MISC"     , (DLGPROC)WINPropWindow::cbDialogMisc},
	{"PROP_PAGE_LCD"      , (DLGPROC)WINPropWindow::cbDialogLcd},
	{"PROP_PAGE_SPEED"    , (DLGPROC)WINPropWindow::cbDialogSpeed},
//	{"PROP_PAGE_FONT"     , WINPropWindow::cbDialogFont},
	{"PROP_PAGE_DIRECTORY", (DLGPROC)WINPropWindow::cbDialogDirectory},
	{"PROP_PAGE_LOG", (DLGPROC)WINPropWindow::cbDialogLog},
};

#define elementof(x) (sizeof(x)/sizeof(*(x)))

/**
 * オプションの表示／変更画面クラスのコンストラクタ
 *
 */
WINPropWindow::WINPropWindow(HINSTANCE hInstance, HWND hWnd)
{
	this->hInstance = hInstance;
	this->hWnd = hWnd;
}

WINPropWindow::~WINPropWindow(void)
{
}

/**
 * show property page
 */
void WINPropWindow::show(void)
{
	saveNowOption();

    PROPSHEETPAGE ps_page;
    PROPSHEETHEADER psh;
    HPROPSHEETPAGE hpsp[elementof(props)];

	int i;
	for(i = 0; i < (int)elementof(props); i++)
	{
		memset(&ps_page, 0, sizeof(PROPSHEETPAGE));

		ps_page.dwSize = sizeof(PROPSHEETPAGE);
		ps_page.dwFlags = PSP_DEFAULT;
		ps_page.hInstance = hInstance;

		ps_page.pszTemplate = props[i].temp;
		ps_page.pfnDlgProc = props[i].funcCallBack;
		hpsp[i]=CreatePropertySheetPage(&ps_page);
	}

	memset(&psh, 0, sizeof(PROPSHEETHEADER));
	psh.dwSize = sizeof(PROPSHEETHEADER);
	psh.dwFlags = PSH_NOAPPLYNOW;
	psh.hInstance = hInstance;
	psh.hwndParent = hWnd;
//	psh.nPages = PROP_PAGE;
	psh.nPages = elementof(props);

	psh.phpage = (HPROPSHEETPAGE *)&hpsp;
//	psh.phpage = hpsp;
	psh.pszCaption = "option";
	PropertySheet(&psh);
}

LRESULT CALLBACK WINPropWindow::cbDialogMisc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		CHECK_WHEN_TRUE( IDC_WARN_ODD,          ::winoption->get_warn_odd_access()  );
		CHECK_WHEN_TRUE( IDC_ENABLE_STATUS_BAR, ::winoption->get_status_bar()       );
		CHECK_WHEN_TRUE( IDC_ENABLE_SOUND,      ::winoption->get_enable_sound()     );
		CHECK_WHEN_TRUE( IDC_ENABLE_CPU_TIMER,  ::winoption->get_enable_cpu_timer() );
		CHECK_WHEN_TRUE( IDC_NO_CAPTION,        ::winoption->get_no_caption()       );
		CHECK_WHEN_TRUE( IDC_ISO8859_1,         ::winoption->get_useIso8859_1()     );

		CHECK_WHEN_TRUE( IDC_ONLY_A_PROCESS,    ::winoption->get_only_a_process()     );

		return TRUE;

	case WM_NOTIFY:
	{
		NMHDR *nmhdr;

		nmhdr = (NMHDR *)lParam;
		if(nmhdr->code == (UINT)PSN_APPLY)
		{
			::winoption->set_warn_odd_access(  TRUE_WHEN_CHECKED( IDC_WARN_ODD          ) );
			::winoption->set_enable_sound(     TRUE_WHEN_CHECKED( IDC_ENABLE_SOUND      ) );
			::winoption->set_enable_cpu_timer( TRUE_WHEN_CHECKED( IDC_ENABLE_CPU_TIMER  ) );
			::winoption->set_status_bar(       TRUE_WHEN_CHECKED( IDC_ENABLE_STATUS_BAR ) );
			::winoption->set_no_caption(       TRUE_WHEN_CHECKED( IDC_NO_CAPTION        ) );
			;;winoption->set_useIso8859_1(     TRUE_WHEN_CHECKED( IDC_ISO8859_1         ) );
			::winoption->set_only_a_process(   TRUE_WHEN_CHECKED( IDC_ONLY_A_PROCESS    ) );

			return TRUE;
		}
	}
	}


	return FALSE;
}


LRESULT CALLBACK WINPropWindow::cbDialogSpeed(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
	{
		SetDlgItemInt(hWnd, IDET_CPU_SPEED,   ::winoption->get_cpu_speed(), FALSE);
		SetDlgItemInt(hWnd, IDET_TIMER_SPEED, ::winoption->get_timer_speed(), TRUE);

		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDPB_RETURN_TO_DEFAULT_CPU:
			SetDlgItemText(hWnd, IDET_CPU_SPEED, "100");
			break;
		case IDPB_RETURN_TO_DEFAULT_TIMER:
			SetDlgItemText(hWnd, IDET_TIMER_SPEED, "0");
			break;
			break;
		}
		return TRUE;
	case WM_NOTIFY:
	{
		NMHDR *nmhdr;

		nmhdr=(NMHDR *)lParam;
		if(nmhdr->code == (UINT)PSN_APPLY)
		{
			BOOL res;
			int i;

			i=GetDlgItemInt(hWnd, IDET_CPU_SPEED, &res, FALSE);
			if(res == TRUE && i > 0)
				::winoption->set_cpu_speed( i );

			i=GetDlgItemInt(hWnd, IDET_TIMER_SPEED, 	&res, TRUE);
			if(res == TRUE)
			{
				if(i > 14)
					i=14;
				else if(i < -14)
					i=-14;
				::winoption->set_timer_speed(i);
			}

			return TRUE;
		}
	}
	}
	return FALSE;
}

#if 0

LRESULT CALLBACK WINPropWindow::cbDialogFont(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char fixed_font_name[256];
	static char prop_font_name[256];

	
	switch(msg)
	{
	case WM_INITDIALOG:
		strcpy(fixed_font_name, aurex_option.option_fixed_font);
		SetDlgItemText(hWnd_dlg, IDET_FIXED_FONT, fixed_font_name);

		strcpy(prop_font_name, aurex_option.option_prop_font);
		SetDlgItemText(hWnd_dlg, IDET_PROP_FONT, prop_font_name);

		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDPB_FIXED_FONT_SELECT:
		{
			LOGFONT lf;
			CHOOSEFONT cf;
			BOOL res;
			
			memset(&cf, 0, sizeof(CHOOSEFONT));

			cf.lStructSize=sizeof(CHOOSEFONT);
			cf.hwndOwner=hWnd_dlg;
			cf.lpLogFont=&lf;
			cf.Flags=CF_SCREENFONTS | CF_EFFECTS;
			cf.rgbColors=RGB(0, 0, 0);
			cf.nFontType=SCREEN_FONTTYPE;

			res=ChooseFont(&cf);
			if(res)
			{
				strcpy(fixed_font_name, lf.lfFaceName);
				SetDlgItemText(hWnd_dlg, IDET_FIXED_FONT, fixed_font_name);
			}
			return TRUE;
		}
		case IDPB_PROP_FONT_SELECT:
		{
			LOGFONT lf;
			CHOOSEFONT cf;
			BOOL res;
			
			memset(&cf, 0, sizeof(CHOOSEFONT));

			cf.lStructSize=sizeof(CHOOSEFONT);
			cf.hwndOwner=hWnd_dlg;
			cf.lpLogFont=&lf;
			cf.Flags=CF_SCREENFONTS | CF_EFFECTS;
			cf.rgbColors=RGB(0, 0, 0);
			cf.nFontType=SCREEN_FONTTYPE;

			res=ChooseFont(&cf);
			if(res)
			{
				strcpy(prop_font_name, lf.lfFaceName);
				SetDlgItemText(hWnd_dlg, IDET_PROP_FONT, prop_font_name);
			}
			return TRUE;
		}
		}
		return TRUE;
	case WM_NOTIFY:
	{
		NMHDR *nmhdr;

		nmhdr=(NMHDR *)lParam;
		if(nmhdr->code == (UINT)PSN_APPLY)
		{
			strcpy(aurex_option.option_fixed_font, fixed_font_name);
			strcpy(aurex_option.option_prop_font, prop_font_name);
			return TRUE;
		}
	}
	}

	return FALSE;
}
#endif

LRESULT CALLBACK WINPropWindow::cbDialogLcd(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:

		SetDlgItemInt(hWnd, IDET_LCD_EXPANSION, ::winoption->get_expansion(), FALSE);
		CHECK_WHEN_TRUE( IDC_AFTER_IMAGE,   ::winoption->get_after_image() );
		CHECK_WHEN_TRUE( IDC_ANTI_ALIASES,  ::winoption->get_anti_alias() );
		CHECK_WHEN_TRUE( IDC_LCD_102X64,    ! ::winoption->get_full_window() );
		CHECK_WHEN_TRUE( IDC_FLIP,          ::winoption->get_flip() );

		CHECK_WHEN_TRUE( IDC_TRANSPARENT,   ::winoption->get_transparent() );
		CHECK_WHEN_TRUE( IDC_ALPHA,         ::winoption->get_alpha() );
		SetDlgItemInt(hWnd, IDET_ALPHA_VALUE, ::winoption->get_alpha_value(), FALSE);

		SetDlgItemInt(hWnd, IDET_AI_VALUE, ::winoption->get_ai_value(), FALSE);

		return TRUE;
	case WM_NOTIFY:
	{
		NMHDR *nmhdr;

		nmhdr=(NMHDR *)lParam;
		if(nmhdr->code == (UINT)PSN_APPLY)
		{
			BOOL res;
			int i;

			i=GetDlgItemInt(hWnd, IDET_LCD_EXPANSION, &res, FALSE);
			if(res == TRUE && i > 0)
				::winoption->set_expansion( i );

			::winoption->set_after_image( TRUE_WHEN_CHECKED( IDC_AFTER_IMAGE  ) );
			::winoption->set_anti_alias(  TRUE_WHEN_CHECKED( IDC_ANTI_ALIASES ) );
			::winoption->set_full_window( ! TRUE_WHEN_CHECKED( IDC_LCD_102X64  ) );
			::winoption->set_flip(        TRUE_WHEN_CHECKED( IDC_FLIP         ) );

			::winoption->set_transparent( TRUE_WHEN_CHECKED( IDC_TRANSPARENT ) );
			::winoption->set_alpha(       TRUE_WHEN_CHECKED( IDC_ALPHA       ) );
			i = GetDlgItemInt(hWnd, IDET_ALPHA_VALUE, &res, FALSE);
			if(res == TRUE && i > 0)
				::winoption->set_alpha_value( i );

			i = GetDlgItemInt(hWnd, IDET_AI_VALUE, &res, FALSE);
			if(res == TRUE && i > 0)
				::winoption->set_ai_value( i );

			return TRUE;
		}
	}

	}
	return FALSE;
}

LRESULT CALLBACK WINPropWindow::cbDialogDirectory(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:

		SetDlgItemText(hWnd, IDET_ROOT_DIRECTORY, ::winoption->get_root_directory() );

		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDPB_ROOT_DIRECTORY:
		{
			char str[1024];
			BROWSEINFO folder_info;
			ITEMIDLIST *p;
			HRESULT hr;
			LPMALLOC pMalloc = NULL;

			folder_info.hwndOwner = hWnd;
			folder_info.pidlRoot  = NULL;
			folder_info.pszDisplayName = str;
			folder_info.lpszTitle = "Select \"B:\" ROOT directory";
			folder_info.ulFlags   = 0;
			folder_info.lpfn      = NULL;
			folder_info.lParam    = (LPARAM)NULL;
			folder_info.iImage    = 0;

			p = SHBrowseForFolder(&folder_info);
			if(p == NULL)
				return FALSE;

			hr = SHGetMalloc(&pMalloc);
            if(hr == E_FAIL)
                return FALSE;

            SHGetPathFromIDList(p, str);

            pMalloc->Free(p);
            pMalloc->Release();

			SetDlgItemText(hWnd, IDET_ROOT_DIRECTORY, str);

			return TRUE;
		}
		return TRUE;
		}
		break;
	case WM_NOTIFY:
	{
		NMHDR *nmhdr;

		nmhdr=(NMHDR *)lParam;
		if(nmhdr->code == (UINT)PSN_APPLY)
		{
			char path[ 1024 ];
			GetDlgItemText(hWnd, IDET_ROOT_DIRECTORY, path, sizeof( path ));
			::winoption->set_root_directory( path );

			return TRUE;
		}
	}
	}

	return FALSE;
}

LRESULT CALLBACK WINPropWindow::cbDialogLog(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static WINPropWindowLog *winPropWindowLog;

	switch(msg)
	{
	case WM_INITDIALOG:

		delete winPropWindowLog;

		winPropWindowLog = new WINPropWindowLog( hWnd );
		winPropWindowLog->enSubclass();

// 現状は無効化しておく
		Button_Enable(GetDlgItem(hWnd, IDC_RADIO_LOG_EVENT), FALSE);

		switch( ::winoption->get_log_type() )
		{
		case Log::TYPE_NULL:
			Button_SetCheck(GetDlgItem(hWnd, IDC_RADIO_LOG_NONE), BST_CHECKED);
			winPropWindowLog->disableLogFile();
			break;
		case Log::TYPE_STDERR:
			Button_SetCheck(GetDlgItem(hWnd, IDC_RADIO_LOG_STDERR), BST_CHECKED);
			winPropWindowLog->disableLogFile();
			break;
		case Log::TYPE_FILE:
			Button_SetCheck(GetDlgItem(hWnd, IDC_RADIO_LOG_FILE), BST_CHECKED);
			winPropWindowLog->enableLogFile();
			break;
		case 3:
			Button_SetCheck(GetDlgItem(hWnd, IDC_RADIO_LOG_EVENT), BST_CHECKED);
			winPropWindowLog->disableLogFile();
			break;
		}

		SetDlgItemInt(hWnd, IDET_LOG_LEVEL, ::winoption->get_log_level(), TRUE);
		SetDlgItemText(hWnd, IDET_LOG_FILE_NAME, ::winoption->get_log_file());

		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDPB_LOG_FILE_SELECT:
		{
			OPENFILENAME FileDialog;

			char currentdirectory[MAX_PATH];
			char exts[256]="";
			char dir[1024] = "";
			char filename[MAX_PATH];

			BOOL res;

	// GetOpenFileName を使用すると勝手にカレントディレクトリを変更されるので
	// 元に戻すために、現在のディレクトリを退避する。
			GetCurrentDirectory(sizeof(currentdirectory), currentdirectory);

#define OPEN_LOG_NAME "Log file (*.*)\0*.*\0\0"
			memcpy(exts, OPEN_LOG_NAME, sizeof( OPEN_LOG_NAME ));

			memset(&FileDialog, 0, sizeof(OPENFILENAME));

			FileDialog.lStructSize		= sizeof(OPENFILENAME);
			FileDialog.lpstrFilter		= exts;
			FileDialog.hwndOwner		= hWnd;
			FileDialog.nMaxFile			= MAX_PATH;
			FileDialog.nMaxFileTitle	= MAX_PATH;
			FileDialog.lpstrInitialDir	= NULL;

			*filename=0;
			FileDialog.lpstrFile		= filename;
			FileDialog.Flags			&= (~OFN_HIDEREADONLY);
			FileDialog.lpstrTitle		= "Log file";

			res = GetOpenFileName(&FileDialog);

			SetCurrentDirectory(currentdirectory);

			if( res == true )
				SetDlgItemText(hWnd, IDET_LOG_FILE_NAME, filename);

			return TRUE;
		}
		return TRUE;
		}
		break;

	case WM_NOTIFY:
		NMHDR *nmhdr;

		nmhdr=(NMHDR *)lParam;
		if(nmhdr->code == (UINT)PSN_APPLY)
		{
			BOOL res;
			int i;

			i=GetDlgItemInt(hWnd, IDET_LOG_LEVEL, &res, TRUE);
			if(res == TRUE)
				::winoption->set_log_level( i );

			if( Button_GetCheck(GetDlgItem(hWnd, IDC_RADIO_LOG_NONE)) == BST_CHECKED )
				::winoption->set_log_type(Log::TYPE_NULL);
			else if( Button_GetCheck(GetDlgItem(hWnd, IDC_RADIO_LOG_STDERR)) == BST_CHECKED )
				::winoption->set_log_type(Log::TYPE_STDERR);
			else if( Button_GetCheck(GetDlgItem(hWnd, IDC_RADIO_LOG_FILE)) == BST_CHECKED )
				::winoption->set_log_type(Log::TYPE_FILE);
			else if( Button_GetCheck(GetDlgItem(hWnd, IDC_RADIO_LOG_EVENT)) == BST_CHECKED )
				::winoption->set_log_type(3);

			char logName[ 1024 ];
			GetDlgItemText(hWnd, IDET_LOG_FILE_NAME, logName, sizeof( logName ));
			::winoption->set_log_file( logName );

			return TRUE;
		}
	}

	return FALSE;
}


/**
 */
void WINPropWindow::saveNowOption(void)
{
	option_b_transparent = ::winoption->get_transparent();
	option_b_alpha       = ::winoption->get_alpha();
	option_i_alpha_value = ::winoption->get_alpha_value();

	option_b_no_caption  = ::winoption->get_no_caption();
	option_b_statusbar   = ::winoption->get_status_bar();

	option_b_full_window = ::winoption->get_full_window();
	option_b_flip        = ::winoption->get_flip();

	option_i_expansion   = ::winoption->get_expansion();

	option_b_antialiases = ::winoption->get_anti_alias();
	option_b_afterimage  = ::winoption->get_after_image();

	strcpy(option_s_root_directory, ::winoption->get_root_directory() );

	option_ai_value = ::winoption->get_ai_value();

	option_log_type = ::winoption->get_log_type();
	strcpy(option_log_file, ::winoption->get_log_file());
}

/**
 * 透過、半透明
 * キャプション
 * ステータスバー
 */
bool WINPropWindow::isChanged_windowStyle(void)
{
	if( option_b_transparent != ::winoption->get_transparent()
		|| option_b_alpha     != ::winoption->get_alpha()
		|| option_i_alpha_value != ::winoption->get_alpha_value()
		|| option_b_no_caption  != ::winoption->get_no_caption()
		|| option_b_statusbar   != ::winoption->get_status_bar()  )
		return true;

	return false;
}

/**
 * 大きさ
 * 縦
 * 倍率
 */
bool WINPropWindow::isChanged_winlcd(void)
{
	if( option_i_expansion != ::winoption->get_expansion()
		|| option_b_full_window != ::winoption->get_full_window()
		|| option_b_flip        != ::winoption->get_flip()        )
		return true;

	return false;
}

/**
 * 倍率
 * アンチエイリアス
 * 残映
 */
bool WINPropWindow::isChanged_cmlcd(void)
{
	if( option_i_expansion   != ::winoption->get_expansion()
		|| option_b_antialiases != ::winoption->get_anti_alias()
		|| option_b_afterimage  != ::winoption->get_after_image()
		|| option_ai_value != ::winoption->get_ai_value() )
		return true;

	return false;
}


/**
 * ログタイプ
 * ログファイル
 * 変更が必要な場合のみ ture を返す
 */
bool WINPropWindow::isChanged_log(void)
{
	if( option_log_type != ::winoption->get_log_type() )
		return true;

	if( option_log_type == Log::TYPE_FILE &&
		strcmp(option_log_file, ::winoption->get_log_file()) != 0 )
		return true;

	return false;
}


/**
 * 仮想ルートディレクトリ
 */
bool WINPropWindow::isChanged_root_directory(void)
{
	if( strcmp( option_s_root_directory, ::winoption->get_root_directory() ) == 0 )
		return false;

	return true;
}
