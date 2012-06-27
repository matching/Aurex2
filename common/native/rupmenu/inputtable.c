/*
 * inputtable.c - rupmenu emulation code (related main functions) (MN10200 code)
 *
 * $Id: inputtable.c,v 1.10 2006/02/16 13:05:12 fumi Exp $
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

/* Todo: 関数名の統一 */


#include <string.h>

#include "rupmenucommon.h"

#include "emu_rupsys.h"
#include "emu_lcdbios.h"

#include "emu_font.h"
#include "emu_gaiji.h"
#include "emu_common.h"


#if 1
#include "itoa.h"
#include <string.h>
#include "emu_lcdbios.h"
#endif

enum RIGHT_ARROW_STATUS
{
	SELECTED,
	UNSELECTED,
};



extern struct RupmenuPlane menuPlane[];
extern int menuPlaneDepth;

#define X_RIGHT_ELEMENT 70
#define X_RIGHT_ARROW 62

static int procItemList(struct RupmenuPlane *localPlane, int iindex, int y);
static int procPlainMenu(struct RupmenuPlane *localPlane, int iindex, int y);


static void show_title(struct RupmenuPlane const *localPlane);
static void show_menu_item(struct RupmenuPlane const *localPlane, int num, int top);
static void show_one_menu_item(struct RupmenuPlane const *localPlane, int n, int show_line);
static void show_one_menu_item_list(struct RupmenuPlane const *localPlane, INPUTITEM *localItem, int y, enum RIGHT_ARROW_STATUS status);
static void show_yajirusi(int num, int top);
static void item_selection(INPUTITEM *p, int show_line);
static void item_unselection(void);

static void menu_init(struct RupmenuPlane const *localPlane, int num, int top);
static void upscroll(struct RupmenuPlane const *localPlane, int top);
static void downscroll(struct RupmenuPlane const *localPlane, int top);


void sub_initInputTable(char *title, INPUTITEM *ii)
{
/* Todo : check overflow */
	currentPlane = menuPlane + menuPlaneDepth;
	menuPlaneDepth++;

	currentPlane->inputItems = ii;
	currentPlane->title = title;
	currentPlane->inputItemIndex = 0;

	currentPlane->select = 0;
}

void sub_endInputTable(void)
{
	menuPlaneDepth--;
	currentPlane = menuPlane + menuPlaneDepth;
}



/* */
static int blinkItemNo;


/*
 * 0, -1 cancel 
 * -2 : 強制的なアプリケーションの終了
 * -3 update
 * -4 cancel
 *
 * キャンセルが一杯あるけど？
 * 0 ==> MENU を押したとき。
 *
 */
int sub_runInputProc(void)
{
	int top;
	int y;
	int key;
	int rst=0;
	int num;

	struct RupmenuPlane *localPlane = currentPlane; /* 再入できなくなるので退避 */
	INPUTITEM *localInput;


	top=0;	/* 一番上に表示しているものの行 */
	y=0;	/* カレントの画面の上からの行 */

	num = localPlane->inputItemIndex;

	menu_init(localPlane, num, top);
	item_selection(localPlane->inputItems + y + top, y);

	for(;;)
	{
		key=emu_Keyin(0xff, -1);
		rst=emu_EventCall(key);
		if(rst == -1)
			return -2; // 終了要求

		item_unselection();

		localInput = localPlane->inputItems + y + top;

		if(key & Bkey_D)
		{
			return 0;
		}

		if( ((key & Bkey_B) && localInput->chmode == CHMODE_PlainMenu )
			|| ( (key & Bkey_rg) && (localInput->chmode == CHMODE_Input || localInput->nextflg == 1 ) ))
		{
			int ret = 0;

#if 0
	{
		char str[100] = "sub Proc, call title (";
		
		strcat(str, localPlane->title);
		strcat(str, ")");
		emu_gv_kput(300, 100, str, 0, 0, 0);
	}
#endif


			if(localInput->disable == 0)
			{
				switch( localInput->chmode )
				{
				case CHMODE_Input:
					ret = procItemList( localPlane, y + top, y );
					break;
				case CHMODE_PlainMenu:
					ret = procPlainMenu( localPlane, y + top, y );
					break;
				}

#if 0
	{
		char str[100] = "sub Proc, return from (";
		
		strcat(str, localPlane->title);
		strcat(str, "), code (");
		util_uitoa_supply_char(ret, str + strlen(str), 4, '0');
		strcat(str, ")");
		emu_gv_kput(300, 100, str, 0, 0, 0);
	}
#endif



		/* 強制終了なら強制終了 */
				if( ret == -2 )
				{
					return -2;
				}

				menu_init(localPlane, num, top);

		//				show_one_menu_item(localPlane, y + top, y);
			}
		}
		else if(key & Bkey_B)
		{
			return -3;
		}


		if(key & Bkey_up)
		{
			if(y == 0)
			{
				if(top > 0)
				{
					upscroll(localPlane, top);
					top--;
					show_yajirusi(num, top);
				}
			}
			else
				y--;
		}
		else if(key & Bkey_dw)
		{
			if(y == 3)
			{
				if(top+4 < num)
				{
					downscroll(localPlane, top);
					top++;
					show_yajirusi(num, top);
				}
			}
			else
				if(y < num-1)
					y++;
		}

		item_selection(localPlane->inputItems + y + top, y);
	}
}


static int procItemList(struct RupmenuPlane *localPlane, int iindex, int y)
{
	int blinkNo;
	int key;
	int rst;
	int keepval;
	INPUTITEM *localInput;

	y = (y + 1) * 13;

	localInput = localPlane->inputItems + iindex;
	keepval = localInput->listval;

	show_one_menu_item_list(localPlane, localInput, y, SELECTED);
	blinkNo = emu_blink(X_RIGHT_ELEMENT, y, 102, y + 13, 0xa0);

	for(;;)
	{
		key=emu_Keyin(0xff, -1);
		rst=emu_EventCall(key);
		if(rst == -1)
		{
			emu_blend(blinkNo);
			localInput->listval = keepval;
			return -2; // 終了要求
		}

		emu_blstop(blinkNo);

		if( key & Bkey_B || key & Bkey_lf ) /* Enter 左でも*/
		{
			emu_blend(blinkNo);
			return 0; /* update */
		}

		if( key & Bkey_D ) /* MENU */
		{
			emu_blend(blinkNo);
			localInput->listval = keepval;
			return -1; /* cancel */
		}

		if( key & Bkey_dw )
		{
			localInput->listval = localInput->listval + 1;
			if( localInput->listval >= localInput->listnum )
				localInput->listval -= localInput->listnum;

		}

		if( key & Bkey_up )
		{
			localInput->listval = localInput->listval - 1;
			if( localInput->listval < 0 )
				localInput->listval += localInput->listnum;
		}

		show_one_menu_item_list(localPlane, localInput, y, SELECTED);
		emu_blstart(blinkNo);
	}

/* not reached */
	return 0;

}

static int procPlainMenu(struct RupmenuPlane *localPlane, int iindex, int y)
{
	INPUTITEM *localInput;

	localInput = localPlane->inputItems + iindex;
	return localInput->menu();
}



static void show_title(struct RupmenuPlane const *localPlane)
{
	int x;
	int width;

// 網模様
	emu_gv_put(0, 0, (char *)mmpGARA, 0);

	width=strlen(localPlane->title)*6;
	x=(103-width)/2;

	emu_gv_clear(x-6, 0, x+width+6, 12);
	emu_gv_kput(x, 0, (char *)localPlane->title, localPlane->font, 0, 0);
}

/*
 * top : top menu index
 */
static void show_menu_item(struct RupmenuPlane const *localPlane, int num, int top)
{
	int i, n;

	n=num-top;
	if(n > 4)
		n=4;

	for(i=0; i < n; i++)
		show_one_menu_item(localPlane, top+i, i);
}

/* show_line 0 to 3
 */
static void show_one_menu_item(struct RupmenuPlane const *localPlane, int n, int show_line)
{
	int y;
	INPUTITEM *p;

	y = (show_line+1)*13;
	p = localPlane->inputItems + n;

	emu_gv_kput(0, y, p->myname, localPlane->font, 0, 0);

	if( p->chmode == CHMODE_Input )
	{
		show_one_menu_item_list(localPlane, p, y, UNSELECTED);
	}


	if( p->nextflg )
	{
		emu_gv_put(98, y+2,	(char *)mmpRYAs, 0);
	}

	if(p->disable == 1)
	{
		int i;
		for(i=0; i < 12; i++)
		{
			if(i & 1)
				emu_gv_line(0, y+i, 101, y+i, 3, 0xaa);
			else
				emu_gv_line(0, y+i, 101, y+i, 3, 0x55);
		}
	}
}

static void show_one_menu_item_list(struct RupmenuPlane const *localPlane, INPUTITEM *localItem, int y, enum RIGHT_ARROW_STATUS status)
{
	if( status == SELECTED )
		emu_gv_sput(X_RIGHT_ARROW, y, GAIJI_RIGHT_ARROW_BLACK, 0);
	else
		emu_gv_sput(X_RIGHT_ARROW, y, GAIJI_RIGHT_ARROW_WHITE, 0);

	emu_gv_kput(X_RIGHT_ELEMENT, y, (char *)get_input_item_string_pointer(localItem, localItem->listval), localPlane->font, 0, 0);
}


static void show_yajirusi(int num, int top)
{
	if(num <= 4)
		return;

/* 下矢印表示 */
	if(top+4 < num)
		emu_gv_put(92, 3, (char *)mmpSITAYA, 0);
	else
		emu_gv_clear(92, 3, 101, 9);

/* 上矢印表示 */
	if(top > 0)
		emu_gv_put(83, 3, (char *)mmpUEYA, 0);
	else
		emu_gv_clear(83, 3, 92, 9);
}


static void item_selection(INPUTITEM *p, int show_line)
{
	int y;

	y = (show_line + 1) * 13;
//	emu_gv_reverse(0, y, 101, y+13);
//	emu_gv_reverse(0, y-1, 101, y+12);

	blinkItemNo = emu_blink(0, y, 102, y + 12, 0xa0);
}

static void item_unselection(void)
{
	emu_blend( blinkItemNo );
}


static void menu_init(struct RupmenuPlane const *localPlane, int num, int top)
{
	emu_gv_place(0, 0);
	emu_gv_clear(0, 0, 101, 63);
	show_title(localPlane);

	show_menu_item(localPlane, num, top);
	show_yajirusi(num, top);
}

static void upscroll(struct RupmenuPlane const *localPlane, int top)
{
	emu_gv_get(0, 13, 101, 51, buffer_for_gv_get);
	show_one_menu_item(localPlane, top-1, 0);
	emu_gv_put(0, 26, buffer_for_gv_get, 0);
}


static void downscroll(struct RupmenuPlane const *localPlane, int top)
{
	emu_gv_get(0, 26, 101, 63, buffer_for_gv_get);
	show_one_menu_item(localPlane, top+4, 3);
	emu_gv_put(0, 13, buffer_for_gv_get, 0);
}

