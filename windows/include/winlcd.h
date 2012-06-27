//-*-c++-*-
// $Id: winlcd.h,v 1.18 2005/01/06 14:21:33 fumi Exp $

#ifndef WINLCD_H
#define WINLCD_H

#include <windows.h>
#include "sdlcd.h"

class WINLcdFont;
class RupAddress;


class WINLcd : public SDLcd
{
  protected:
	unsigned char *dibData;
	HDC hDC;
	HDC hDC_wnd;

	int wwidth;
	int wheight;

	int byte_per_line;

  private:
	HBITMAP hBitmap;
	HGDIOBJ prev_obj;

	BITMAPINFO *bitmapInfo;
	HWND hWnd;


  public:
	WINLcd(HWND hWnd, CMLcd *cmlcd, int width, int height);
	WINLcd(HWND hWnd, CMLcd *cmlcd);
	virtual ~WINLcd(void);
	virtual void updateWindow(void);

	bool hasFontAa(void);

	SDLcdFont *createSDLcdFont(int width, int height, int prop, int gap, bool latin);
	SDLcdFont *createSDLcdFontAa(int width, int height, int prop, int gap, bool latin);

	void adjustWindowSize(void);
	void adjustWindowSize(int w, int h);
  protected:
	void init(void);

  private:
	unsigned char createColor(int bit);
};


class WINLcdFlip : public WINLcd
{
  public:
	WINLcdFlip(HWND hWnd, CMLcd *cmlcd);
	void updateWindow(void);
};

class WINLcd102x64 : public WINLcd
{
  private:
	RupAddress *systemAddress;

  public:
	WINLcd102x64(RupAddress *systemAddress, HWND hWnd, CMLcd *cmlcd);
	~WINLcd102x64(void);
	void updateWindow(void);
};



#endif
