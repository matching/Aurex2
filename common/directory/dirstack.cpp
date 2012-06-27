/*
 * dirstack.cpp - Directory string parser module
 *
 * $Id: dirstack.cpp,v 1.12 2006/02/16 18:19:40 fumi Exp $
 *
 * Copyright (C) 2004, 2005 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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
#include <string.h>
#include <assert.h>

#include "dirstack.h"
#include "directoryelement.h"


DirStack::DirStack(void)
{
	head.child=head.parent=&head;
}

DirStack::DirStack(char const *dir, char dirsep)
{
	head.child=head.parent=&head;
	makeDirStack(dir, dirsep);
}

DirStack::~DirStack(void)
{
	DirectoryElement *p, *parent;

	p=head.parent;
	while(p != &head)
	{
		parent=p->parent;
		delete p;
		p=parent;
	}
}

void DirStack::concat(DirStack *p)
{
	DirectoryElement *lowest;
	DirectoryElement *plowest;

	lowest=head.parent;
	plowest=p->head.parent;

	lowest->child=p->head.child;
	p->head.child->parent=lowest;

	plowest->child=&head;
	head.parent=plowest;

	p->head.child=p->head.parent=&p->head;
}

/**
 * 最後に dirsep はつかない
 */
void DirStack::toFullPath(char *dst, char dirsep)
{
	DirectoryElement *p;

	p=head.child;
	for(; p != &head; p=p->child)
	{
		*dst=dirsep;
		strcpy(dst+1, p->directory);
		dst=strchr(dst, 0);
	}
	*dst=0;
}

void DirStack::print(void)
{
	DirectoryElement *p;

	p=head.parent;
	for(; p != &head; p=p->parent)
	{
		printf("%s\n", p->directory);
	}
}

void DirStack::push(DirectoryElement *de)
{
	DirectoryElement *parent;

	parent=head.parent;

	parent->child=de;
	de->parent=parent;
	head.parent=de;
	de->child=&head;
}


void DirStack::push(char const *dir)
{
#ifdef DEBUG
	fprintf(stderr, "DirStack::push (%s)\n", dir);
	fflush(stderr);
#endif

	DirectoryElement *p;
	p=new DirectoryElement(dir);

	push(p);
}
void DirStack::push(char const *start, char const *end)
{
#ifdef DEBUG
	fprintf(stderr, "DirStack::push (%s, %s)\n", start, end);
	fflush(stderr);
#endif
	assert( start );


	DirectoryElement *p;
	p=new DirectoryElement(start, end);

	push(p);
}

/**
 * 正規化
 * ./ を無視
 * ../ で一つ上に行く
 */
void DirStack::normalize(void)
{
	DirectoryElement *p;
	char *c;

	p=head.child;
	for(; p != &head; p=p->child)
	{
		if(p->directory[0]  == '.')
		{
			c=p->directory+1;
			if(*c == '.')
				c++;
			if(*c == 0)
			{
				int offset;

				offset=c-p->directory;

				if(offset == 1)
				{
			// . を発見削除
					DirectoryElement *parent;
					parent=p->parent;

					parent->child=p->child;
					p->child->parent=p->parent;

					delete p;

					p=parent;
				}
				else
				{
			// .. を発見一つまえまで削除
					DirectoryElement *parent;
					parent=p->parent;
					if(parent == &head)
					{
				// root directory での .. → .. だけ削除
						DirectoryElement *parent;
						parent=p->parent;

						parent->child=p->child;
						p->child->parent=p->parent;

						delete p;

						p=parent;
					}
					else
					{
						DirectoryElement *parentparent;

						parentparent=parent->parent;

						parentparent->child=p->child;
						p->child->parent=parentparent;

						delete p;
						delete parent;

						p=parentparent;
					}
				}
			}
		}
	}
}

/**
 *
 */
void DirStack::makeDirStack(char const *dir, char dirsep)
{
	int state;
	state=0;
	char const *start = NULL;

	while(*dir)
	{
		switch(state)
		{
		case 0:
			if(*dir != dirsep)
			{
				start=dir;
				state=1;
			}
			break;
		case 1:
			if(*dir == dirsep)
			{
				push(start, dir);
				state=0;
			}
			break;
		}
		dir++;
	}

	switch(state)
	{
	case 0:
		break; // do nothing
	case 1:
		push(start, dir);
	}
}

DirStack *DirStack::rest(DirStack *dst)
{
	assert( dst );

	DirectoryElement *p, *q;

	p=head.child;
	q=dst->head.child;
	for(; p != &head; p=p->child, q=q->child)
	{
		if(q == &dst->head)
			return NULL;

		if(!p->equal(q))
			return NULL;
	}

	DirStack *ds;

	ds=new DirStack();
	for(; q != &dst->head; q=q->child)
	{
		ds->push(q->directory);
	}
	ds->normalize();
	return ds;
}

bool DirStack::isSetup(void)
{
	if(head.child != &head)
		return true;
	return false;
}

#if 0
DirStack *DirStack::clone(void)
{
	DirStack *d = new DirStack();

	DirectoryElement *p;
	p = head.child;
	for(; p != &head; p=p->child)
		d->push(p->directory);

	return d;
}
#endif


#if UNIT_TEST

#include "directory.h"
#include "dosdirectory.h"
#include "unixdirectory.h"
#include "ruputerdirectory.h"
#include "directoryconverter.h"

/*
 * to compile
 * $ g++ -Wall -DDEBUG -DUNIT_TEST -mno-cygwin -I../include dirstack.cpp ruputerdirectory.cpp dosdirectory.cpp directory.cpp directoryconverter.cpp directoryelement.cpp
 */

/**
 * 単体試験用
 */
int main(int argc, char **argv)
{
	Directory *p;
	char str[1024]="";
	int i;

	argc--;
	argv++;

	if(argc <= 0)
		return 0;

// 仮想ルートディレクトリ
	p=new DosDirectory("c:\\c\\d");

	if( p->isAbsoluteDirectory(*argv) )
		fprintf(stderr, " %s is absolute directory...\n", *argv);
	else
		fprintf(stderr, " %s is not absolute directory...\n", *argv);


	p->getFullFileName(*argv, str);
	fprintf(stderr, "0: getFullFileName (%s)\n", str);
	fflush(stderr);

//	p=new DosDirectory();
//	p=new UnixDirectory("/home/fumi");

	RuputerDirectory *r;

//	r=new RuputerDirectory(p, "b:\\bar");
	r=new RuputerDirectory(p);
#if 0
	r->chdir(*argv);
	return 0;
#endif

	DirectoryConverter *local2ruputer, *ruputer2local;

	local2ruputer = new DirectoryConverter(p, r);
	i = local2ruputer->convert(*argv, str);
	fprintf(stderr, "1: local to ruputer %d (%s)\n", i, str);


	*str=0;
	ruputer2local = new DirectoryConverter(r, p);
	i = ruputer2local->convert(*argv, str);
	fprintf(stderr, "2: ruputer to local %d (%s)\n", i, str);


#if 0
	p->chdir(argv[1]);
	p->getdir(str);
	printf("%s\n", str);
#endif

	return 0;

}



#endif
