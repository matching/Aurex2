/* $Id: rupmenucommon.h,v 1.5 2006/02/16 13:05:13 fumi Exp $ */

#ifndef RUPMENUCOMMON_H
#define RUPMENUCOMMON_H


#define MAX_INPUT_ITEM 10

typedef struct {
	int  myid;
	char *myname;
	int	 ydot;
	int  xdir;
	int  chmode;
	int  dspmode;
	int  disable;
	int  xlistL;
	int  xlistR;
	char *list;
	int  listnum;
	int  listlen;
	int  listval;
	int  listtgl;
	int  (*listEx)(int,int,int);
	void (*listcallback)(int,int);
	int  (*menu)(void);
	int  nextflg;
	char *extinfo;
} INPUTITEM;


struct RupmenuPlane
{
	char const *title;

	INPUTITEM *inputItems;
	int inputItemIndex;

	int font;

	int select;
};


#define CHMODE_Input     0
#define CHMODE_PlainMenu 1
#define CHMODE_InputEx   2
#define CHMODE_Time      3
#define CHMODE_TimeZone  4
#define CHMODE_Date      5
#define CHMODE_userext   6



extern struct RupmenuPlane *currentPlane;
extern int last_key_code;

int getMenuId(void);
INPUTITEM *getInputItem(int id);
int searchStringPointer(char *myname);
char const *get_input_item_string_pointer(INPUTITEM *localInput, int no);


#endif
