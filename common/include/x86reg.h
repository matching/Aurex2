/* -*-c++-*- */
// $Id: x86reg.h,v 1.4 2004/04/29 09:44:32 fumi Exp $

#ifndef X86REG_H
#define X86REG_H

class MemoryForInside;

enum X86RegType {
	X86REG_AX,
	X86REG_BX,
	X86REG_CX,
	X86REG_DX,
	X86REG_SI,
	X86REG_DI,
	X86REG_DS,
	X86REG_ES,
	X86REG_CS,
	X86REG_CFLAG,

	X86REG_AL, X86REG_AH,
	X86REG_BL, X86REG_BH,
	X86REG_CL, X86REG_CH,
	X86REG_DL, X86REG_DH,
};


class X86Reg
{
  private:
	MemoryForInside *mem;

  public:
	X86Reg(unsigned long addr);
	~X86Reg(void);

	unsigned short read_AX(void);
	unsigned short read_BX(void);
	unsigned short read_CX(void);
	unsigned short read_DX(void);
	unsigned short read_SI(void);
	unsigned short read_DI(void);
	unsigned short read_DS(void);
	unsigned short read_ES(void);
	unsigned short read_CS(void);
	unsigned short read_CFLAG(void);

	unsigned char read_AL(void);
	unsigned char read_AH(void);
	unsigned char read_BL(void);
	unsigned char read_BH(void);
	unsigned char read_CL(void);
	unsigned char read_CH(void);
	unsigned char read_DL(void);
	unsigned char read_DH(void);

	void write_AX(unsigned short dat);
	void write_BX(unsigned short dat);
	void write_CX(unsigned short dat);
	void write_DX(unsigned short dat);
	void write_SI(unsigned short dat);
	void write_DI(unsigned short dat);
	void write_DS(unsigned short dat);
	void write_ES(unsigned short dat);
	void write_CS(unsigned short dat);
	void write_CFLAG(unsigned short dat);

	void write_AL(unsigned char dat);
	void write_AH(unsigned char dat);
	void write_BL(unsigned char dat);
	void write_BH(unsigned char dat);
	void write_CL(unsigned char dat);
	void write_CH(unsigned char dat);
	void write_DL(unsigned char dat);
	void write_DH(unsigned char dat);
};

#define OFFSET_AX 0
#define OFFSET_BX 2
#define OFFSET_CX 4
#define OFFSET_DX 6
#define OFFSET_SI 8
#define OFFSET_DI 10
#define OFFSET_DS 12
#define OFFSET_ES 14
#define OFFSET_CS 16
#define OFFSET_CFLAG 18

#define OFFSET_AL (OFFSET_AX)
#define OFFSET_AH (OFFSET_AX+1)
#define OFFSET_BL (OFFSET_BX)
#define OFFSET_BH (OFFSET_BX+1)
#define OFFSET_CL (OFFSET_CX)
#define OFFSET_CH (OFFSET_CX+1)
#define OFFSET_DL (OFFSET_DX)
#define OFFSET_DH (OFFSET_DX+1)


#if 0
/************************************************
 * 8086 CPU word registers						*
 ***********************************************/
struct wordregs86 {
	unsigned short ax;
	unsigned short bx;
	unsigned short cx;
	unsigned short dx;
	unsigned short si;
	unsigned short di;
	unsigned short ds;
	unsigned short es;
	unsigned short cs;
	unsigned short cflag;
	unsigned short reserve[3];
};
/************************************************
 * 8086 CPU byte registers						*
 ***********************************************/
struct byteregs86 {
	unsigned char al, ah;
	unsigned char bl, bh;
	unsigned char cl, ch;
	unsigned char dl, dh;
};

/************************************************
 * 8086 CPU registers structure for dos call. 	*
 ***********************************************/
typedef union regs86 {
	struct wordregs86 x;
	struct byteregs86 h;
}REGS86;
#endif

#endif
