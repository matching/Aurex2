/*
 * winruputer.cpp - windows ruputer emulation
 *
 * $Id: winruputer.cpp,v 1.48 2007/07/28 17:36:52 fumi Exp $
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

#include "winruputer.h"
#include "keybuffer.h"
#include "state.h"
#include "winlcd.h"
#include "winlcdfactory.h"
#include "directory.h"
#include "frame.h"
#include "option.h"
#include "optionitem.h"
#include "ruputerexception.h"
#include "filerinimgr.h"
#include "log.h"
#include "exffile.h"
#include "cmlcd.h"

#include <stdio.h>


#include "winresource.h"
#include "winfindfile.h"
#include "winoption.h"
#include "winpropwindow.h"
#include "winsound.h"

#include "winlogoutputevent.h"

#include <assert.h>
#include <commctrl.h>

static char *szClassName=CLASS_NAME;

static KeyBuffer *keybuf;
static WINRuputer *winruputer;

/* extern in winoption.h */
WINOption *winoption;

/**
 *
 *
 */

WINRuputer::WINRuputer(HINSTANCE hInstance, int nCmdShow, Frame *frame, Directory *sddir, WINOption *option, UFilerIni *uini) : Ruputer(frame, sddir, option, uini)
{
	::winruputer = this;
	::winoption = option;
	::keybuf = getKeyBuffer();

	this->winlcd = NULL;
	this->winsound = new WINSound();

	setSDSound( this->winsound );

	this->hInstance = hInstance;
	this->winlcdfact = new WINLcdFactory(winoption);

	initWindows(nCmdShow);

	char extlist[1024];
	filerini->getExtList( extlist );
	createGetFileNamePattern( extlist );

	setupLog();
}

WINRuputer::~WINRuputer(void)
{
	delete winsound;

	delete winlcdfact;
	delete winlcd;
}
/**
 * window クラスの作成
 *
 */
int WINRuputer::initWindows(int nCmdShow)
{
	registerClassName();
	createWindow(nCmdShow);

	changeWINLcd();
#if 0
	winlcd = winlcdfact->getWINLcd(winruputer->getSystemAddress(), hWnd, getCMLcd());
	setSDLcd(winlcd);
	adjustWindow();
#endif

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return 0;
}

void WINRuputer::registerClassName(void)
{
    WNDCLASS wc;

	memset(&wc, 0, sizeof(WNDCLASS));

    wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WINRuputer::WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;        //インスタンス
    wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = ::szClassName;
	wc.lpszMenuName=NULL;

	RegisterClass(&wc);
}

void WINRuputer::createWindow(int nCmdShow)
{
	if( getWindowStyleHasEx() )
	{
		hWnd = CreateWindowEx(WS_EX_LAYERED,
							  ::szClassName,
							  "Aurex",
							  getWindowStyle(),
							  ::winoption->get_window_x_pos(),
							  ::winoption->get_window_y_pos(),
							  winlcdfact->getWidth(),
							  winlcdfact->getHeight(),
							  NULL,
							  NULL,
							  hInstance,
							  NULL);

		updateLayeredAttribute();
	}
	else
	{
		hWnd = CreateWindow(::szClassName,
							"Aurex",
							getWindowStyle(),
							::winoption->get_window_x_pos(),
							::winoption->get_window_y_pos(),
							winlcdfact->getWidth(),
							winlcdfact->getHeight(),
							NULL,
							NULL,
							hInstance,
							NULL);
	}
	updateAlwaysOnTop();

	InitCommonControls();
	hWndStatus = CreateWindowEx(0,
								STATUSCLASSNAME,
								NULL,
								WS_CHILD | SBARS_SIZEGRIP | CCS_BOTTOM,
								0,
								0,
								0,
								0,
								hWnd,
								(HMENU)ID_STATUS,
								hInstance,
								NULL);

	SendMessage(hWndStatus, SB_SIMPLE, TRUE, 0L);
	updateStatusBar();
}

int WINRuputer::EventProcedure(void)
{
	if( ::winoption->get_status_bar() )
	{
		static int flag;

		if(flag == 0)
		{
			int fps;
			int gfps;
			char buf[256];

			fps = frame->getFrameRateTimes1000();
			gfps = frame->getGraphicFrameRateTimes1000();

			sprintf(buf, "%d.%03d / %d.%03d fps", gfps / 1000, gfps % 1000, fps / 1000, fps % 1000);

			SendMessage(hWndStatus, SB_SETTEXT, (WPARAM)255, (LPARAM)buf);
		}

		flag++;
		if( flag >= 32 )
			flag = 0;
	}



	int res;
	MSG msg;

/* イベントを取り出す */
	res=PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	if(res == 0)
		return 0;

	TranslateMessage(&msg);
	DispatchMessage(&msg);


	return 1;
}

LRESULT WINRuputer::WndProc(HWND hWnd_local, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDM_NEW_LOAD:
		{
			char filename[MAX_PATH];
			if(WINRuputer::getFileName(hWnd_local, filename))
			{
				::winruputer->requestNewLoad(filename);
			}
			break;
		}
		case IDM_ALWAYS_TOP:
			if( ::winoption->get_always_on_top() == true )
				::winoption->set_always_on_top(false);
			else
				::winoption->set_always_on_top(true);
			::winruputer->updateAlwaysOnTop();
			break;

		case IDM_RELOAD:
			::winruputer->requestReload();
			break;
		case IDM_SETTING:
		{
			WINPropWindow prop(::winruputer->getHInstance(), hWnd_local);

			prop.show();

			if( prop.isChanged_log() )
				::winruputer->setupLog();

	// 順番は大事!
			if( prop.isChanged_cmlcd() )
				::winruputer->getCMLcd()->setExpansion( ::winoption->get_expansion() );
			if( prop.isChanged_winlcd() || prop.isChanged_windowStyle() )
			{
				::winruputer->updateWindowStyle();
				::winruputer->changeWINLcd();
			}
	// 無条件に更新
			::winruputer->frame->updateFrameTime();

			if( prop.isChanged_root_directory() )
			{
				Directory *dir = ::winruputer->getLocalDirectory();

				dir->setdir( ::winoption->get_root_directory() );
				if( dir->isSetupDirectory() && ! dir->isExistentDirectory() )
				{
					char str[1024];
					dir->getdir(str);
					fprintf(stderr, "Warning ! Directory %s is not exists\n", str);
					fflush(stderr);
			// 続行する？
				}
			}
			break;
		}
		case IDM_VERSION:
#define HELP_STRING "\
usage:\n\
  Enter	: w\n\
  EL	: s\n\
  Menu	: x\n\
  Filer	: z\n\
  up/down/left/right arrow\n"

			MessageBox(hWnd_local, HELP_STRING, "HELP", MB_OK);
			break;

		case IDM_END:
			::winruputer->requestDestroy();
			break;


		case IDM_KEY_ENTER:
			::keybuf->enQueueKey(Bkey_B | KEY_PUSH_DOWN);
			::keybuf->enQueueKey(Bkey_B | KEY_PUSH_UP);
			break;
		case IDM_KEY_EL:
			::keybuf->enQueueKey(Bkey_C | KEY_PUSH_DOWN);
			::keybuf->enQueueKey(Bkey_C | KEY_PUSH_UP);
			break;
		case IDM_KEY_MENU:
			::keybuf->enQueueKey(Bkey_D | KEY_PUSH_DOWN);
			::keybuf->enQueueKey(Bkey_D | KEY_PUSH_UP);
			break;
		case IDM_KEY_FILER:
			::keybuf->enQueueKey(Bkey_A | KEY_PUSH_DOWN);
			::keybuf->enQueueKey(Bkey_A | KEY_PUSH_UP);
			break;
		case IDM_KEY_UP:
			::keybuf->enQueueKey(Bkey_up | KEY_PUSH_DOWN);
			::keybuf->enQueueKey(Bkey_up | KEY_PUSH_UP);
			break;
		case IDM_KEY_RIGHT:
			::keybuf->enQueueKey(Bkey_rg | KEY_PUSH_DOWN);
			::keybuf->enQueueKey(Bkey_rg | KEY_PUSH_UP);
			break;
		case IDM_KEY_DOWN:
			::keybuf->enQueueKey(Bkey_dw | KEY_PUSH_DOWN);
			::keybuf->enQueueKey(Bkey_dw | KEY_PUSH_UP);
			break;
		case IDM_KEY_LEFT:
			::keybuf->enQueueKey(Bkey_lf | KEY_PUSH_DOWN);
			::keybuf->enQueueKey(Bkey_lf | KEY_PUSH_UP);
			break;
		}
		::winruputer->frame->sync();
		break;
// end case WM_COMMAND:
	case WM_KEYDOWN:
		switch(wParam)
		{
	// ゲームの入力
		case VK_LEFT:
			::keybuf->enQueueKey(Bkey_lf | KEY_PUSH_DOWN);
			break;
		case VK_DOWN:
			::keybuf->enQueueKey(Bkey_dw | KEY_PUSH_DOWN);
			break;
		case VK_UP:
			::keybuf->enQueueKey(Bkey_up | KEY_PUSH_DOWN);
			break;
		case VK_RIGHT:
			::keybuf->enQueueKey(Bkey_rg | KEY_PUSH_DOWN);
			break;
		case 'Z':
			::keybuf->enQueueKey(Bkey_A | KEY_PUSH_DOWN);
			break;
		case 'W':
			::keybuf->enQueueKey(Bkey_B | KEY_PUSH_DOWN);
			break;
		case 'S':
			::keybuf->enQueueKey(Bkey_C | KEY_PUSH_DOWN);
			break;
		case 'X':
			::keybuf->enQueueKey(Bkey_D | KEY_PUSH_DOWN);
			break;

		case VK_TAB:
			::winruputer->requestReload();
			break;
		}
		break;
	case WM_KEYUP:
		switch(wParam)
		{
		case VK_LEFT:
			::keybuf->enQueueKey(Bkey_lf | KEY_PUSH_UP);
			break;
		case VK_UP:
			::keybuf->enQueueKey(Bkey_up | KEY_PUSH_UP);
			break;
		case VK_DOWN:
			::keybuf->enQueueKey(Bkey_dw | KEY_PUSH_UP);
			break;
		case VK_RIGHT:
			::keybuf->enQueueKey(Bkey_rg | KEY_PUSH_UP);
			break;
		case 'Z':
			::keybuf->enQueueKey(Bkey_A | KEY_PUSH_UP);
			break;
		case 'W':
			::keybuf->enQueueKey(Bkey_B | KEY_PUSH_UP);
			break;
		case 'S':
			::keybuf->enQueueKey(Bkey_C | KEY_PUSH_UP);
			break;
		case 'X':
			::keybuf->enQueueKey(Bkey_D | KEY_PUSH_UP);
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		SendMessage(hWnd_local, WM_SYSCOMMAND, SC_MOVE | 2, 0);
		break;

	case WM_RBUTTONDOWN:
		showMouseRButtonMenu(wParam, lParam);
		break;

#if 0
	case WM_LBUTTONDBLCLK:
	{
		OptionItem *o;

		o=::ruputer->optionmgr->getOptionItem(OPT_NO_CAPTION);
		if(o)
		{
			bool b;
			b=o->getBoolValue();
			b^=1;
			o->setBoolValue(b);

			::ruputer->optionmgr->callCallbackFunction();
		}
		break;
	}
#endif
	case WM_CREATE:
		DragAcceptFiles( hWnd_local, TRUE );
		break;
	case WM_CLOSE:
		::winruputer->requestDestroy();
		break;
	case WM_DESTROY:
	{
		RECT rect;
//		OptionItem *oi;

		GetWindowRect(hWnd_local, &rect);

#ifdef DEBUG
		fprintf(stderr, "!! get window rect (%d, %d)\n", rect.left, rect.top);
		fflush(stderr);
#endif

		::winoption->set_window_x_pos(rect.left);
		::winoption->set_window_y_pos(rect.top);

#if 0
		oi=::ruputer->optionmgr->getOptionItem(OPT_X_POS);
		if(oi)
			oi->setIntValue(rect.left);

		oi=::ruputer->optionmgr->getOptionItem(OPT_Y_POS);
		if(oi)
			oi->setIntValue(rect.top);
#endif

	}
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		SendMessage(::winruputer->hWndStatus, WM_SIZE, wParam, lParam);
		break;

	case WM_COPYDATA:
		COPYDATASTRUCT *pcds;
		pcds = (COPYDATASTRUCT *)lParam;
		::winruputer->requestNewLoad( (char *)pcds->lpData );
		break;

	case WM_DROPFILES:
	{
		HDROP hDrop;
		UINT i;
		char filename[1024];

		hDrop = (HDROP)wParam;
		i = DragQueryFile( hDrop, 0xFFFFFFFF, NULL, 0 );
		DragQueryFile( hDrop, i - 1, filename, sizeof(filename) );
		DragFinish( hDrop );

		::winruputer->requestNewLoad( filename );

		break;
	}

	default:
/* bimyo */
		return DefWindowProc(hWnd_local, msg, wParam, lParam);
    }
    return 0;
}

void WINRuputer::showMouseRButtonMenu(WPARAM wParam, LPARAM lParam)
{
	HMENU hMenu;
	HMENU hSubmenu;
	POINT pt;

	pt.x=LOWORD(lParam);
	pt.y=HIWORD(lParam);

	hMenu=LoadMenu(winruputer->hInstance, "MOUSE_MENU");
	hSubmenu=GetSubMenu(hMenu, 0);

	if( ::winoption->get_always_on_top() == true )
		CheckMenuItem(hMenu, IDM_ALWAYS_TOP, MF_BYCOMMAND | MF_CHECKED);
	else
		CheckMenuItem(hMenu, IDM_ALWAYS_TOP, MF_BYCOMMAND | MF_UNCHECKED);

	ClientToScreen(winruputer->hWnd, &pt);

	TrackPopupMenu(hSubmenu, TPM_LEFTALIGN, pt.x, pt.y, 0, winruputer->hWnd, NULL);
	DestroyMenu(hMenu);
}

char *WINRuputer::getFileNamePattern = 0;

void WINRuputer::createGetFileNamePattern(char *list)
{
	char ext[256] = "exf";

	int n, len;

	len = strlen(list);
	n = (len + 3 ) / 4; // 拡張子の数
	getFileNamePattern = new char[ ( n + 1 ) * 14 ];

	char *out = getFileNamePattern;
	for(;;)
	{
		*out++ = '*';
		*out++ = '.';
		strcpy(out, ext);
		out += strlen(ext);

		if( *list == 0 )
			break;

		char *p = ext;
		while( *list != 0 && *list != ',' )
			*p++ = *list++;
		*p = 0;

		if( *list != 0)
			list++; // うーん苦肉だなー。


		*out++ = ';';
	}
	*out = 0;

}


bool WINRuputer::getFileName(HWND hWnd, char *filename)
{
	OPENFILENAME FileDialog;

	char currentdirectory[MAX_PATH];

	char exts[256]="";
	char dir[1024] = "";
	BOOL res;


// GetOpenFileName を使用すると勝手にカレントディレクトリを変更されるので
// 元に戻すために、現在のディレクトリを退避する。
	GetCurrentDirectory(sizeof(currentdirectory), currentdirectory);

	if( ::winruputer->getLocalDirectory()->isSetupDirectory() )
	{
		::winruputer->getLocalDirectory()->getdir(dir);
	}

//	memcpy(exts, "(*.exf)\0*.exf\0\0", 14);
#define OPEN_NAME "Ruputer/onHand executable file"
	memcpy(exts, OPEN_NAME, sizeof( OPEN_NAME ) );
	strcpy(exts + sizeof( OPEN_NAME ), getFileNamePattern);
	memcpy(exts + sizeof( OPEN_NAME ) + strlen(getFileNamePattern), "\0\0", 2);

	memset(&FileDialog, 0, sizeof(OPENFILENAME));

	FileDialog.lStructSize		= sizeof(OPENFILENAME);
	FileDialog.lpstrFilter		= exts;
	FileDialog.hwndOwner		= hWnd;
	FileDialog.nMaxFile			= MAX_PATH;
	FileDialog.nMaxFileTitle	= MAX_PATH;
	FileDialog.lpstrInitialDir	= dir;

	*filename=0;
	FileDialog.lpstrFile		= filename;
	FileDialog.Flags			&= (~OFN_HIDEREADONLY);
	FileDialog.lpstrTitle		= "open file";

	res=GetOpenFileName(&FileDialog);

	SetCurrentDirectory(currentdirectory);

	return res;
}

SDFindFile *WINRuputer::createSDFindFile(struct dta_t *dta_area, char const *pathname, unsigned char search_attribute)
{
	return new WINFindFile(dta_area, pathname, search_attribute);

}


/**
 * 現在の Window style を返す
 * @retval DWORD window style
 */
DWORD WINRuputer::getWindowStyle(void)
{
	DWORD style;

	style = WS_POPUP | WS_BORDER | WS_SYSMENU ;
	if( ! ::winoption->get_no_caption() )
		style |= WS_CAPTION;

	return style;
}

/**
 * Window style EX を使用するようなオプションの状態であるか否か
 * @retval true ex 状態を持つ
 * @retval false 持たない
 */
bool WINRuputer::getWindowStyleHasEx(void)
{
	return ::winoption->get_transparent() | ::winoption->get_alpha();
}

/**
 * 半透明値や白抜け情報を登録する
 */
void WINRuputer::updateLayeredAttribute(void)
{
	if( ::winoption->get_transparent() )
		SetLayeredWindowAttributes(hWnd, 0xffffff, 0, LWA_COLORKEY);
	else if( ::winoption->get_alpha() )
		SetLayeredWindowAttributes(hWnd, 0, ::winoption->get_alpha_value(), LWA_ALPHA);
	else
		assert( 0 );
}

void WINRuputer::updateWindowStyle(void)
{
	if( getWindowStyleHasEx() )
	{
		SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		updateLayeredAttribute();
	}
	else
	{
		SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
	}
	SetWindowLong(hWnd, GWL_STYLE, getWindowStyle());

	updateStatusBar();
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}

void WINRuputer::updateStatusBar(void)
{
	if( ::winoption->get_status_bar() )
		ShowWindow(hWndStatus, SW_SHOW);
	else
		ShowWindow(hWndStatus, SW_HIDE);
}


/**
 *
 *
 */
void WINRuputer::updateAlwaysOnTop(void)
{
	if( ::winoption->get_always_on_top() == true )
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	else
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}


#if 0
void WINRuputer::OptionIsChanged(Option *options)
{
	Ruputer::OptionIsChanged(options);

	int recreationflag=0;

	OptionItem *i, *j;

	i=options->getOptionIfChanged(OPT_NO_CAPTION);
	if(i)
	{
#ifdef DEBUG
		fprintf(stderr, "no caption !!r (%d) %d %x\n", i->getBoolValue(), i->changed(), windowstyle);
		fflush(stderr);
#endif
		if(i->getBoolValue())
			windowstyle&=~WS_CAPTION;
		else
			windowstyle|=WS_CAPTION;
		SetWindowLong(hWnd, GWL_STYLE, windowstyle);
		if(winlcd)
			winlcd->adjustWindowSize();
		SetWindowPos(hWnd, 0, 0, 0, 0, 0, SWP_NOCOPYBITS | SWP_FRAMECHANGED | SWP_DRAWFRAME | SWP_NOSIZE | SWP_NOMOVE);
		ShowWindow(hWnd, SW_SHOW);
	}


	i=options->getOptionIfChanged(OPT_ALPHA);
	if(i)
	{
		if(i->getBoolValue())
		{
			j=options->getOption(OPT_ALPHA_VALUE);
			if(j)
			{
				SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
				SetLayeredWindowAttributes(hWnd, 0, j->getIntValue(), LWA_ALPHA);

				recreationflag=1;
			}
		}
		else
		{
			SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
			recreationflag=1;
		}
	}

	j=options->getOptionIfChanged(OPT_TRANSPARENT);
	if(j)
	{
		if(j->getBoolValue())
		{
			SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
			SetLayeredWindowAttributes(hWnd, 0xffffff, 0, LWA_COLORKEY);
			recreationflag=1;
		}
		else
		{
			if(recreationflag == 0)
			{
				recreationflag=1;
				SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
			}
		}
	}

	i=options->getOptionIfChanged(OPT_ALWAYS_ON_TOP);
	if(i)
	{
#ifdef DEBUG
		fprintf(stderr, "!! option top most %d\n", i->getBoolValue());
		fflush(stderr);
#endif
		if(i->getBoolValue())
			SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else
			SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	i=options->getOption(OPT_X_POS);
	j=options->getOption(OPT_Y_POS);
	if(i && j)
	{
		if(i->changed() || j->changed())
		{
			winxpos=i->getIntValue();
			winypos=j->getIntValue();

			SetWindowPos(hWnd, 0, winxpos, winypos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
	}

//	if(recreationflag == 1)
	{
		delete winlcd;
		winlcd=winlcdfact->getWINLcd(winruputer->getSystemAddress(), hWnd, cmlcd);
		setSDLcd(winlcd);
	}
}


#endif



/**
 *
 */
bool WINRuputer::printErrorMessageForUndefinedInstruction(UndefinedInstructionException *e)
{
	char regsString[1024];

	e->getRegisterString(regsString);

	return printErrorMessage("Undefined Instruction Error !!", regsString);
}

/**
 *
 */
bool WINRuputer::printErrorMessageForOddAccess(OddAccessException *e)
{
	char regsString[1024];

	sprintf(regsString, "Effective Address 0x%lx\n\n", e->getEffectiveAddress());
	e->getRegisterString(regsString + strlen(regsString));

	return printErrorMessage("A muiti-byte transfer instructon to odd address !!", regsString);
}

bool WINRuputer::printErrorMessage(char const *title, char const *str)
{
	int n;
	char out[1024];

	strcpy(out, str);
	strcat(out, "\n\nDo you continue to emulate ?");

	n = MessageBox(hWnd, out, title, MB_YESNO);
	if(n == IDYES)
		return true;
	return false;
}


HINSTANCE WINRuputer::getHInstance(void)
{
	return this->hInstance;
}

WINLcdFactory *WINRuputer::getWINLcdFactory(void)
{
	return this->winlcdfact;
}

void WINRuputer::setWINLcd(WINLcd *winlcd)
{
	this->winlcd = winlcd;
}


void WINRuputer::adjustWindow(void)
{
	if( ::winoption->get_status_bar() )
	{
		RECT rect;
		GetWindowRect(hWndStatus, &rect);
		winlcd->adjustWindowSize(0, rect.bottom - rect.top);
	}
	else
		winlcd->adjustWindowSize();
}


void WINRuputer::changeWINLcd(void)
{
	delete winlcd;

	winlcd = winlcdfact->getWINLcd( ::winruputer->getSystemAddress(), hWnd, ::winruputer->getCMLcd());

//	::winruputer->setWINLcd(winlcd);
	setSDLcd(winlcd);
	adjustWindow();
}


int WINRuputer::load(char const *filename) throw()
{

	try{
		Ruputer::load(filename);
	}
	catch( ExfFileInvalidException &e ) {
		char message[1024];
		sprintf(message, "exf file is invalid (%s)", filename);
		MessageBox(hWnd, message, "Error", MB_OK);
	}
	catch( ExfFileNotFoundException &e ) {
		char message[1024];
		sprintf(message, "A exf file associated with (%s) is not found", filename);
		MessageBox(hWnd, message, "Error", MB_OK);
	}
	catch( AssociatedFileNotInVirtualDirectoryException &e ) {
		char message[1024];
		sprintf(message, "A file (%s) is out of virtual directory.", filename);
		MessageBox(hWnd, message, "Error", MB_OK);
	}

	if( exffile )
	{
		setIcon();
	}

	return 0;
}

void WINRuputer::setIcon(void)
{
	HICON hIcon;

// list (12x12)
	unsigned char list_icon[12 * 12];

	exffile->getMiniIcon(list_icon);
	hIcon = makeIcon(list_icon, 12, 12, 16, 16);
 	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	CloseHandle(hIcon);

// big (31x21)
	unsigned char big_icon[64 * 64];
	int w, h;

	exffile->getLargeIcon(big_icon, &w, &h);

	hIcon = makeIcon(big_icon, w, h, 32, 32);

 	SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	CloseHandle(hIcon);
}


HICON WINRuputer::makeIcon(unsigned char *icon, int swidth, int sheight, int dwidth, int dheight)
{
	assert( swidth <= dwidth );
	assert( sheight <= dheight );

	int byte_per_line;
	int size;

	byte_per_line = ( dwidth + 7 ) / 8;
	size = byte_per_line * dheight;

	unsigned char *fore = new unsigned char[size];
	unsigned char *mask = new unsigned char[size];

	memset(fore, 0, size);
	memset(mask, 0, size);

	int x, y;

	for(y = 0; y < sheight; y++)
	{
		for(x = 0; x < swidth; x++)
		{
			if( icon[ x + y * swidth] == 0)
				fore[ x / 8 + y * byte_per_line ] |= 1 << ( 7 - x % 8 );
		}

		for(; x < dwidth; x++)
			mask[ x / 8 + y * byte_per_line ] |= 1 << ( 7 - x % 8 );
	}
	for(; y < dheight; y++)
	{
		for(x = 0; x < byte_per_line; x++)
			mask[ y * byte_per_line + x] = 0xff;
	}


	HICON hIcon;
	hIcon = CreateIcon(hInstance, dwidth, dheight, 1, 1, mask, fore);

	delete[] fore;
	delete[] mask;

	return hIcon;
}


void WINRuputer::setupLog(void)
{
	switch( ::winoption->get_log_type() )
	{
	case 3: /* event */
		Log::getInstance()->setLogOutput( new WINLogOutputEvent() );
		break;
	}

	Ruputer::setupLog();
}


void WINRuputer::destory(void)
{
	MSG msg;

	DestroyWindow(hWnd);

	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
