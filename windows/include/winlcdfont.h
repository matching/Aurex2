// -*-c++-*-
// $Id: winlcdfont.h,v 1.6 2006/01/21 10:02:03 fumi Exp $

#ifndef WINLCDFONT_H
#define WINLCDFONT_H

#include <windows.h>
#include "sdlcdfont.h"

class CesSjis;

class WINLcdFont : public SDLcdFont
{
  protected:
	HDC hDC;
	HDC hDC_wnd;

	HBITMAP hBitmap;
	unsigned char *dibData;
	HGDIOBJ prev_obj;
	HFONT hFont;
	BITMAPINFO *bitmapInfo;
	HWND hWnd;

	int gap;
	int fwidth;
	int fheight;
	int prop;

  public:

	WINLcdFont(HDC hDC_wnd, int width, int height, int prop, int gap, char const *fixedfont="Courier New", char const *propfont="Arial Narrow", DWORD charset=ANSI_CHARSET);
//	WINLcdFont(HDC hDC_wnd, int width, int height, int prop, int gap, char const *fixedfont="Arial", char const *propfont="Arial", DWORD charset=ANSI_CHARSET);
	virtual ~WINLcdFont(void);
	virtual unsigned char **drawString(char const *str, int *resultwidth);
	virtual void freeBuffer(unsigned char **buf);
};

class WINLcdFontAa : public WINLcdFont
{
  public:
	WINLcdFontAa(HDC hDC_wnd, int width, int height, int prop, int gap);
	WINLcdFontAa(HDC hDC_wnd, int width, int height, int prop, int gap, char const *fixedfont, char const *propfont, DWORD charset);
	~WINLcdFontAa(void);
	virtual unsigned char **drawString(char const *str, int *resultwidth);
	virtual void freeBuffer(unsigned char **buf);
};

class WINLcdFontSjis : public WINLcdFont
{
  private:
	CesSjis *sjis;

  public:
	WINLcdFontSjis(HDC hDC_wnd, int width, int height, int prop, int gap);
	~WINLcdFontSjis(void);
	virtual unsigned char **drawString(char const *str, int *resultwidth);
};


class WINLcdFontAaSjis : public WINLcdFontAa
{
  private:
	CesSjis *sjis;

  public:
	WINLcdFontAaSjis(HDC hDC_wnd, int width, int height, int prop, int gap);
	~WINLcdFontAaSjis(void);
	virtual unsigned char **drawString(char const *str, int *resultwidth);
};

#endif
