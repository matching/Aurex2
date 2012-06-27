// -*-c++-*-
// $Id: mncore.h,v 1.6 2004/11/10 04:02:35 fumi Exp $

#ifndef MNCORE_H
#define MNCORE_H

class Register{
  public:
	unsigned long d[4];
	unsigned long a[4];

	unsigned long pc;
	unsigned short mdr;
	union processor_status_word{
		unsigned short row;
		struct psw_bit{

#define ZF 0x0001
#define NF 0x0002
#define CF 0x0004
#define VF 0x0008
#define ZX 0x0010
#define NX 0x0020
#define CX 0x0040
#define VX 0x0080

#ifndef HOST_IS_BIG_ENDIAN
			unsigned short zf:1;
			unsigned short nf:1;
			unsigned short cf:1;
			unsigned short vf:1;
			unsigned short zx:1;
			unsigned short nx:1;
			unsigned short cx:1;
			unsigned short vx:1;

			unsigned short im0:1;
			unsigned short im1:1;
			unsigned short im2:1;
			unsigned short ie:1;
			unsigned short s0:1;
			unsigned short s1:1;
			unsigned short _bit14:1;
			unsigned short _bit15:1;
#else
			unsigned short im0:1;
			unsigned short im1:1;
			unsigned short im2:1;
			unsigned short ie:1;
			unsigned short s0:1;
			unsigned short s1:1;
			unsigned short _bit14:1;
			unsigned short _bit15:1;

			unsigned short zf:1;
			unsigned short nf:1;
			unsigned short cf:1;
			unsigned short vf:1;
			unsigned short zx:1;
			unsigned short nx:1;
			unsigned short cx:1;
			unsigned short vx:1;
#endif

		}bit;
	}psw;

	unsigned long convFromPsw(unsigned short d);
	unsigned short convToPsw(unsigned long d);
	void toString(char *out);
};

#endif
