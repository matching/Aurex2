// $Id: print-mn-code.c,v 1.1 2004/11/14 14:03:09 fumi Exp $

#include <stdio.h>

unsigned char imm[]={0x12, 0x34, 0x56};
unsigned char nops[]={0xf6, 0xf6, 0xf6, 0xf6};
static int index;

void code2(int op1);
void code3(int op1);
void code4(int op1);
void code5(int op1);


FILE *openfile(void)
{
	char fn[256];
	sprintf(fn, "ins/%04x.dat", index++);
	//	sprintf(fn, "ins/foo.dat", op);
	return fopen(fn, "wb+");

}

int main(void)
{
	int op1;
	int op2;
	char fn[256];
	FILE *fp;

	for(op1=0; op1 <= 0xff; op1++)
	{
		if(op1 >= 0xf0 && op1 <= 0xf3)
			code2(op1);
		else if(op1 == 0xf4)
			code5(op1);
		else if(op1 == 0xf5)
			code3(op1);
		else if(op1 == 0xf7)
			code4(op1);
		else
		{
			FILE *fp;

			fp=openfile();

			fwrite(&op1, 1, 1, fp);
			fwrite(imm, 1, 3, fp);

			fclose(fp);
		}
	}
	return 0;

}

void code2(int op1)
{
	int op2;

	for(op2=0; op2 < 256; op2++)
	{
		FILE *fp;

		fp=openfile();
		fwrite(&op1, 1, 1, fp);
		fwrite(&op2, 1, 1, fp);
		fclose(fp);
	}
}


void code5(int op1)
{
	int op2;

	for(op2=0; op2 < 256; op2++)
	{
		FILE *fp;

		fp=openfile();
		fwrite(&op1, 1, 1, fp);
		fwrite(&op2, 1, 1, fp);
		fwrite(imm, 1, 3, fp);
		fclose(fp);
	}
}



void code3(int op1)
{
	int op2;

	for(op2=0; op2 < 256; op2++)
	{
		FILE *fp;

		fp=openfile();
		fwrite(&op1, 1, 1, fp);
		fwrite(&op2, 1, 1, fp);
		fwrite(imm, 1, 3, fp);
		fclose(fp);
	}
}

void code4(int op1)
{
	int op2;

	for(op2=0; op2 < 256; op2++)
	{
		FILE *fp;

		fp=openfile();
		fwrite(&op1, 1, 1, fp);
		fwrite(&op2, 1, 1, fp);
		fwrite(imm, 1, 3, fp);
		fclose(fp);
	}
}
