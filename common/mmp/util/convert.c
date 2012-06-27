/*
 * $Id: convert.c,v 1.2 2004/03/07 05:29:35 fumi Exp $
 *
 * Copyright (C) 2001, 2002, 2004 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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
 *
 *

/*
 * aurexrom.txt
 *  -> aurexmmp.dat
 *  -> aurexgaiji.dat
 * 変換ツール
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STATE_NO 0
#define STATE_GAIJI 1
#define STATE_MMP 2

struct mmp_data{
	unsigned long address;
	int data_size;
	unsigned char *data;

	struct mmp_data *next;
};

static struct mmp_data mmp_head; /* list head (dummy) */


struct gaiji_data{
	unsigned short code;
	unsigned char data[24];

	struct gaiji_data *next;
};
static struct gaiji_data gaiji_head; /* list head (dummy) */
static int gaiji_count;




static void ins_mmp_data(struct mmp_data * const p)
{
	struct mmp_data *prev;
	struct mmp_data *c;

	prev=&mmp_head;
	c=mmp_head.next;
	for(; c != NULL; prev=c, c=c->next)
		if(c->address > p->address)
			break;

	p->next=c;
	prev->next=p;
}

static void to_mmp(char (*buf)[102], int width, int height, unsigned char *data)
{
	int x, y;
	int i;

	*(unsigned short *)data=width;
	data+=2;
	*(unsigned short *)data=height;
	data+=2;

	for(y=0; y < height; y+=8)
	{
		for(x=0; x < width; x++, data++)
		{
			*data=0;
			for(i=0; i < 8 && y+i < height; i++)
			{
				if(buf[y+i][x] == '@')
					*data|=1 << i;
			}
		}
	}

}

static void mmp_end(char (*buf)[102], unsigned long address, int width, int height)
{
	int data_size;
	unsigned char *data;
	struct mmp_data *p;

	data_size=((height+7)/8)*width+4;
	data=malloc(data_size);
	if(data == NULL)
	{
		fprintf(stderr, "error!!");
		exit(1);
	}

	to_mmp(buf, width, height, data);

	p=malloc(sizeof(struct mmp_data));
	if(p == NULL)
	{
		fprintf(stderr, "error !!");
		exit(1);
	}

	p->address=address;
	p->data_size=data_size;
	p->data=data;

	ins_mmp_data(p);
}


static void to_gaiji(char (*buf)[102], unsigned char *data)
{
//	int i, j;
	int x, y;
	int i;
#if 0
	for(i=0; i < 12; i++, data++)
	{
		*data=0;
		for(j=0; j < 12; j++)
			if(buf[i][j] == '@')
				*data|=1 << j;
	}
#endif


	for(y=0; y < 12; y+=8)
	{
		for(x=0; x < 12; x++, data++)
		{
			*data=0;
			for(i=0; i < 8 && y+i < 12; i++)
			{
				if(buf[y+i][x] == '@')
					*data|=1 << i;
			}
		}
	}

}

static void ins_gaiji(struct gaiji_data * const p)
{
	struct gaiji_data *prev;
	struct gaiji_data *c;

	prev=&gaiji_head;
	c=gaiji_head.next;
	for(; c != NULL; prev=c, c=c->next)
		if(c->code > p->code)
			break;

	p->next=c;
	prev->next=p;
}

static void gaiji_end(char (*buf)[102], unsigned long address)
{
	struct gaiji_data *p;

	p=malloc(sizeof(struct gaiji_data));
	if(p == NULL)
	{
		fprintf(stderr, "malloc error ");
		exit(1);
	}

	p->code=(short)address;
	to_gaiji(buf, p->data);

	ins_gaiji(p);
	gaiji_count++;
}

static int check_line(char *line, int width)
{
	while(*line == '.' || *line == '@')
	{
		line++;
		width--;
	}

	if(width == 0)
		return 0;

	fprintf(stderr, "error invalid character\n");
	return -1;
}


void check(void)
{
	FILE *fp;
	char str[1024];
	char *p, *q;
	int width, height;
	int state=STATE_NO, i;
	int line=0;
	char buf[64][102];
	unsigned long address;

	fp=stdin;

	while(fgets(str, sizeof(str), fp) != NULL)
	{
		line++;
		if(*str == '#')
			continue;

		switch(state)
		{
		case STATE_GAIJI:
			if(memcmp(str, "end", 3) == 0)
			{
				if(i != 12)
				{
					fprintf(stderr, "error invalid height : %d\n", line);
					exit(1);
				}
				gaiji_end(buf, address);
				state=STATE_NO;
				break;
			}

			if(check_line(str, 12) == -1)
			{
				fprintf(stderr, "error : %d\n", line);
				exit(1);
			}
			memcpy(buf[i++], str, 102);

			break;
		case STATE_MMP:
			if(memcmp(str, "end", 3) == 0)
			{
				if(i != height)
				{
					fprintf(stderr, "error invalid height : %d\n", line);
					exit(1);
				}
				mmp_end(buf, address, width, height);
				state=STATE_NO;
				break;
			}

			if(check_line(str, width) == -1)
			{
				fprintf(stderr, "error : %d\n", line);
				exit(1);
			}
			memcpy(buf[i++], str, 102);

			break;
		case STATE_NO:
			if(memcmp(str, "gaiji", 5) == 0)
			{
				state=STATE_GAIJI;
				address=strtol(str+5, &p, 16);
				if(address == 0 || isprint(*p))
				{
					fprintf(stderr, "line : %d error", line);
					exit(1);
				}
				i=0;
			}
			else if(memcmp(str, "mmp", 3) == 0)
			{

				state=STATE_MMP;
				address=strtol(str+3, &p, 16);
				if(*p != ' ')
				{
					fprintf(stderr, "line : %d error", line);
					exit(1);
				}
				width=strtol(p, &q, 10);
				if(*q != ' ')
				{
					fprintf(stderr, "line : %d error", line);
					exit(1);
				}
				height=strtol(q, &p, 10);
				if(isgraph(*p))
				{
					fprintf(stderr, "line : %d error", line);
					exit(1);
				}
				i=0;
			}
			break;
		}
	}
}

static void putbyte(FILE *fp, unsigned char c)
{
	static int i;

	if(i % 20 == 19)
	{
		putc('\n', fp);
		i=0;
	}
	fprintf(fp, "0x%02x, ", c);
	i++;
}

static void out_mmp(void)
{
	FILE *fp;
	struct mmp_data *p;
	unsigned long address;

	fp=stdout;

	p=mmp_head.next;
	address=p->address;

	fprintf(fp, "0x%08x\n", address);

	for(; p != NULL; p=p->next)
	{
//		printf("mmp struct data\n  address %lx\n  size %d\n\n", p->address, p->data_size);

		if(address > p->address)
		{
			fprintf(stderr, "overlapped mmp data %lx\n", address);
			exit(1);
		}

// padding
		while(address < p->address)
		{
		//			putc(0, fp);
			putbyte(fp, 0);
			address++;
		}

		{
			int i;
			for(i=0; i < p->data_size; i++)
			{
		//			fprintf(fp, "%02x, ", p->data[i]);
				putbyte(fp, p->data[i]);
			}
		}
//		fwrite(p->data, 1, p->data_size, fp);

		address+=p->data_size;
	}

//	fclose(fp);

}

static void out_gaiji(void)
{
	FILE *fp;
	struct gaiji_data *p;
	unsigned long address;

	fp=stdout;

	p=gaiji_head.next;
//	fwrite(&gaiji_count, 1, 4, fp);
	for(; p != NULL; p=p->next)
	{

		fprintf(fp, "{0x%04x, {", p->code);
		int i;
		for(i=0; i < 24; i++)
		{
			fprintf(fp, "0x%02x, ",  p->data[i]);
		}
		fprintf(fp, "}}, \n");

//		fwrite(&p->code, 1, 2, fp);
//		fwrite(p->data, 1, 24, fp);
	}
//	fclose(fp);
}


int main(int argc, char **argv)
{
	int mmpmode;
	int gaiji;

	argc--;
	if(argc <= 0)
	{
		fprintf(stderr, "error\n usage %s -{mmp|gaiji}", *argv);
		return 1;
	}

	argv++;

	mmpmode=0;
	if(strcmp(*argv, "-mmp") == 0)
		mmpmode=1;
	else if(strcmp(*argv, "-gaiji") == 0)
		;

	check();

	if(mmpmode == 1 && mmp_head.next)
		out_mmp();

	if(mmpmode == 0 && gaiji_head.next)
		out_gaiji();

	return 0;
}





