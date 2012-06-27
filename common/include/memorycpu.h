// -*-c++-*-
// $Id: memorycpu.h,v 1.7 2004/11/01 11:12:16 fumi Exp $

#ifndef MEMORYCPU_H
#define MEMORYCPU_H

#include "memory.h"

class Port;
class InternalFont;
class RomMmp;

class MemoryForCpu : public Memory
{
  private:
	Port *port;
	InternalFont *ifont;
	RomMmp *rommmp;

  public:
	MemoryForCpu(Port *port, InternalFont *ifont, RomMmp *rommp);
	~MemoryForCpu(void);

	unsigned char read8_0e(unsigned long addr);
	unsigned long read8_se(unsigned long addr);

	unsigned short read16_0e(unsigned long addr);
	unsigned long read16_se(unsigned long addr);

	unsigned long read24(unsigned long addr);

	void write8(unsigned long addr, unsigned char data);
	void write8(unsigned long addr, unsigned long data);
	void write16(unsigned long addr, unsigned short data);
	void write16(unsigned long addr, unsigned long data);
	void write24(unsigned long addr, unsigned long data);


	unsigned long to24bit(unsigned long d);
	unsigned long signExtend8(unsigned long d);
	unsigned long signExtend16(unsigned long d);

	signed char readDisplacement8(unsigned long addr);
	short readDisplacement16(unsigned long addr);
	long readDisplacement24(unsigned long addr);

	unsigned char readAbsolute8(unsigned long addr);
	unsigned short readAbsolute16(unsigned long addr);
	unsigned long readAbsolute24(unsigned long addr);

	unsigned long readImmediate8(unsigned long addr);
	unsigned long readImmediate16(unsigned long addr);
	unsigned long readImmediate24(unsigned long addr);

	void warnOddAddress(unsigned long addr);
};

/*
 * 速度を意識しインライン関数にしておく
 */
inline unsigned long MemoryForCpu::to24bit(unsigned long d)
{
	return d & TO24BIT;
}

inline unsigned long MemoryForCpu::signExtend8(unsigned long d)
{
	if(d & (1 << 7))
		return d | 0xffff00UL;
	return d & 0xff;
//	return to24bit((unsigned long)(signed char)(d & 0xff));
}

inline unsigned long MemoryForCpu::signExtend16(unsigned long d)
{
	if(d & (1 << 15))
		return d | 0xff0000UL;
	return d & 0xffff;
//	return to24bit((unsigned long)(signed short)(d & 0xffff));
}

inline signed char MemoryForCpu::readDisplacement8(unsigned long addr)
{
	return (signed char)readMemory8(addr);
}
 inline short MemoryForCpu::readDisplacement16(unsigned long addr)
{
	return (short)readMemory16(addr);
}

inline long MemoryForCpu::readDisplacement24(unsigned long addr)
{
	unsigned long dat;

	dat=readMemory24(addr);
	if(dat & (1 << 23))
		return dat | 0xff000000UL;

	return dat;
}

inline unsigned char MemoryForCpu::readAbsolute8(unsigned long addr)
{
	return readMemory8(addr);
}

inline unsigned short MemoryForCpu::readAbsolute16(unsigned long addr)
{
	return readMemory16(addr);
}

inline unsigned long MemoryForCpu::readAbsolute24(unsigned long addr)
{
	return readMemory24(addr);
}

inline unsigned long MemoryForCpu::readImmediate8(unsigned long addr)
{
	unsigned char dat;

	dat=readMemory8(addr);
	if(dat & (1 << 7))
		return (unsigned long)dat | 0xffff00UL;
	return dat;
}

inline unsigned long MemoryForCpu::readImmediate16(unsigned long addr)
{
	unsigned short dat;

	dat=readMemory16(addr);
	if(dat & (1 << 15))
		return (unsigned long)dat | 0xff0000UL;
	return dat;
}

inline unsigned long MemoryForCpu::readImmediate24(unsigned long addr)
{
	return readMemory24(addr);
}

#endif
