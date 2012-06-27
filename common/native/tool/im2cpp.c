
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
INIT 部  何書いてあっても無視
%
INFO 部
outfile ファイル名
size ADDR にメモリ展開するサイズの最大値
%
VERB 部
そのまま 出力される。
%
ADDR 部
ex.
0x001000 ファイル名
  で、0x1000 にファイルの内容が出力される。
0x001100 その下にアドレスだけ書くと、そこのアドレスを越えていないかをチェックする
%
@ が行頭にくるとあらゆる場面でコメント扱い
*/


#define STATE_INIT 0
#define STATE_INFO 1
#define STATE_VERB 2
#define STATE_ADDR 3
#define STATE_END  4

unsigned char *outdata;
unsigned char *flag;
unsigned long max;
char outfilename[256];
unsigned long size;
FILE *out;

void write_outdata(unsigned long addr, char *filename)
{
	FILE *fp;
	int c;

	fp=fopen(filename, "rb");
	if(fp == NULL)
	{
		fprintf(stderr, "file open error (%s)\n", filename);
		exit(1);
	}

	while((c=getc(fp)) != EOF)
	{
		outdata[addr++]=c;
		if(addr > size)
		{
			fprintf(stderr, "memory size over ..\n");
			exit(1);
		}
	}

	if(addr > max)
		max=addr;

	fclose(fp);
}


unsigned long dec_hex_to_num(char *str)
{
	unsigned long ret;

	if(*str == '0' && *(str+1) == 'x')
		return strtoul(str+2, NULL, 16);
	else
		return strtoul(str, NULL, 10);
}


void remove_tail_space(char *p)
{
	char *q;

	q=strchr(p, 0);
	q--;
	while(q >= p && (*q == '\n' || *q == '\r' || *q == ' '))
		*q--=0;
}

void parse(FILE *in)
{
	char str[1024];
	char *p;
	int state=STATE_INIT;

	while(fgets(str, sizeof(str), in) != NULL)
	{
		if(*str == '@')
			continue;

		if(state >= STATE_END)
			break;

		switch(state)
		{
		case STATE_INIT:
			if(*str == '%')
				state=STATE_INFO;
			break; // do nothing
		case STATE_INFO:
			p=strchr(str, ' ');
			if(p)
			{
				*p=0;

				remove_tail_space(p+1);

				if(strcmp(str, "outfile") == 0)
					strcpy(outfilename, p+1);
				else if(strcmp(str, "size") == 0)
					size=dec_hex_to_num(p+1);
			}
			else if(*str == '%')
			{
				if(size == 0)
				{
					fprintf(stderr, "size is not defined");
					exit(1);
				}
				else if(*outfilename == 0)
				{
					fprintf(stderr, "outfile is not defined");
					exit(1);
				}

				outdata=(unsigned char *)malloc(size);
				out=fopen(outfilename, "w");
				if(out == NULL)
				{
					fprintf(stderr, "outfile cannot open (%s)", outfilename);
					exit(1);
				}

				state=STATE_VERB;
			}

			break;
		case STATE_VERB:
			if(*str == '%')
				state=STATE_ADDR;
			else
				fputs(str, out);
			break;
		case STATE_ADDR:
		{
			p=strchr(str, ' ');
			if(p)
			{
				unsigned long addr;
				*p=0;
				addr=dec_hex_to_num(str);
				remove_tail_space(p+1);
				write_outdata(addr, p+1);
			}
		}
		}
	}
}


void output(void)
{
	int i;

	fputs("{\n", out);

	for(i=0; i < max; i++)
	{
		fprintf(out, "0x%02x,", outdata[i]);

		if(i % 20 == 19)
			putc('\n', out);
	}

	fputs("\n};\n", out);

}



void syori(FILE *in)
{
	parse(in);
	output();
	fclose(out);
}

int main(int argc, char **argv)
{
	FILE *in;

	argc--;
	argv++;

	if(argc <= 0)
	{
		fprintf(stderr, "arg is little");
		return 1;
	}

	in=fopen(*argv, "r");
	if(in == NULL)
	{
		fprintf(stderr, "file is not open %s", *argv);
		return 1;
	}

	syori(in);

	fclose(in);
	return 0;
}
