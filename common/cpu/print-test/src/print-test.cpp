// -*-mode:c++; coding:euc-jp-unix-*-
// $Id: print-test.cpp,v 1.2 2004/11/14 15:48:52 fumi Exp $

#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "port.h"
#include "memory.h"
#include "memorycpu.h"


#define INITIAL_PC 0x1000
#define INITIAL_SP 0x2000

void load(char const *filename)
{
	FILE *fp;
	char *p;

	p = (char *)Memory::getMemoryPointer(INITIAL_PC);

	fp = fopen(filename, "rb");
	if( fp == NULL )
		exit(1);

// 100 bytes もないけど構わない
	fread(p, 1, 100, fp);

	fclose(fp);
}


int main(int argc, char **argv)
{
	argc--;
	argv++;
	if( argc <= 0 )
		return 1;

	MemoryForCpu *memcpu;
	Cpu *cpu;
	memcpu = new MemoryForCpu(new Port(NULL), NULL, NULL);
	cpu = new Cpu(INITIAL_PC, INITIAL_SP, NULL, memcpu);

	load(*argv);

	cpu->printMnemonic(NULL);

	return 0;
}
