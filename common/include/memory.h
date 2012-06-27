/* -*-c++-*- */
/* $Id: memory.h,v 1.13 2004/04/29 10:22:21 fumi Exp $ */

#ifndef MEMORY_H
#define MEMORY_H

#define MEMORY_SIZE 0x120000


#define TO24BIT 0xffffff


class MemoryBase
{
public:
	unsigned char *memory;

  public:
	MemoryBase(unsigned char *_memory);

/* ↓ ここから */
 	unsigned char readMemory8(unsigned long addr);
	unsigned short readMemory16(unsigned long addr);
	unsigned long readMemory24(unsigned long addr);
	unsigned long readMemory32(unsigned long addr);

	void writeMemory8(unsigned long addr, unsigned char data);
	void writeMemory16(unsigned long addr, unsigned short data);
	void writeMemory24(unsigned long addr, unsigned long data);
	void writeMemory32(unsigned long addr, unsigned long data);
/* ↑ ここまでのメソッドで
 * エンディアンネスを吸収する
 */
	int isAddressOver(unsigned long addr);

//	virtual void *getMemoryPointer(unsigned long addr);
};

class Memory : public MemoryBase
{
  public:
	static unsigned char main_memory[MEMORY_SIZE];
  public:
	Memory(void) : MemoryBase(main_memory)
	{
	}

	static void *getMemoryPointer(unsigned long addr);

};

class MemoryForInside : public MemoryBase
{
  private:
	unsigned long baseaddr;

  public:
	MemoryForInside(void);
	MemoryForInside(unsigned long base);
	MemoryForInside(unsigned char *buf);

	unsigned char read8_u(unsigned long offset);
	signed   char read8_s(unsigned long offset);

	unsigned short read16_u(unsigned long offset);
	signed   short read16_s(unsigned long offset);

	unsigned long read24(unsigned long offset);
	unsigned long read32(unsigned long offset);

	void write8(unsigned long offset, unsigned char data);
	void write16(unsigned long offset, unsigned short data);
	void write24(unsigned long offset, unsigned long data);
	void write32(unsigned long offset, unsigned long data);


//	void *getMemoryPointer(unsigned long offset);
};




#endif

