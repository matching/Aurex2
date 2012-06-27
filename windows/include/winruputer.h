//-*-c++-*-
#ifndef WINRUPUTER_H
#define WINRUPUTER_H

#include "ruputer.h"

#include <windows.h>
#include <stdio.h>
#define CLASS_NAME "aurex(rupemu-0.1.0)"

class WINLcdFactory;
class WINLcd;
class WINOption;

class WINSound;

class WINRuputer : public Ruputer{

  private:
	HDC hDc;
	HWND hWnd;
	HINSTANCE hInstance;
	WINLcd *winlcd;
	WINLcdFactory *winlcdfact;

	WINSound *winsound;

	HWND hWndStatus;

	static char *getFileNamePattern;

  public:
	WINRuputer(HINSTANCE hInstance, int nCmdShow, Frame *frame, Directory *sddir, WINOption *option, UFilerIni *uini);
	~WINRuputer(void);


	SDFindFile *createSDFindFile(struct dta_t *dta_area, char const *pathname, unsigned char search_attribute);
	int load(char const *filename) throw();

	bool printErrorMessageForUndefinedInstruction(UndefinedInstructionException *e);
	bool printErrorMessageForOddAccess(OddAccessException *e);

  private:
	DWORD getWindowStyle(void);
	bool getWindowStyleHasEx(void);
	void updateWindowStyle(void);
	void updateLayeredAttribute(void);
	void updateAlwaysOnTop(void);
	void updateStatusBar(void);

	HINSTANCE getHInstance(void);
	WINLcdFactory *getWINLcdFactory(void);
	void setWINLcd(WINLcd *winlcd);
	void adjustWindow(void);
	void changeWINLcd(void);

	int initWindows(int nCmdShow);
	void registerClassName(void);
	void createWindow(int nCmdShow);

	void setupLog(void);

	int EventProcedure(void);
//	void updateWindow(void);
	static LRESULT CALLBACK WndProc(HWND hWnd_local, UINT msg, WPARAM wParam, LPARAM lParam);
	static void showMouseRButtonMenu(WPARAM wParam, LPARAM lParam);
	static bool getFileName(HWND hWnd, char *filename);
	static void createGetFileNamePattern(char *list);

	bool printErrorMessage(char const *title, char const *str);

	void setIcon(void);
	HICON makeIcon(unsigned char *icon, int swidth, int sheight, int dwidth, int dheight);


	void destory(void);
};


#endif

