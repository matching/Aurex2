/* -*-mode:c++; coding:euc-jp-*- */
// $Id: lcdfunc.h,v 1.14 2006/01/21 18:04:34 fumi Exp $

#ifndef LCDFUNC_H
#define LCDFUNC_H

#include "memory.h"
//class Memory;
class CMLcd;
class RupAddress;

class LcdFunc
{
  protected:
	MemoryForInside *mem;
	CMLcd *cmlcd;

  public:
	LcdFunc(CMLcd *_cmlcd, MemoryForInside *_mem);
	virtual ~LcdFunc(void);
	virtual unsigned long callLcdFunc(void)=0;
};

class LcdFuncMoji
{
  public:
	void convertSCOSII(char *str);
	int isSCOSII(int size);
};

class LcdFuncRect : public LcdFunc
{
  public:
	LcdFuncRect(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFunc(_cmlcd, _mem)
	{
	}
	int getX1(void);
	int getY1(void);

	int getX2(void);
	int getY2(void);

	unsigned char getStyle(void)
	{
		return mem->read8_u(12);
	}
	unsigned long getStyle32(void)
	{
		return mem->read32(12);
	}

	int getLogic(void)
	{
		return mem->read16_u(10);
	}
};

class LcdFuncPoint : public LcdFunc
{
  public:
	LcdFuncPoint(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFunc(_cmlcd, _mem)
	{
	}

	int getX(void);
	int getY(void);
};

class LcdFuncOnlyShort : public LcdFunc
{
  public:
	LcdFuncOnlyShort(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFunc(_cmlcd, _mem)
	{
	}
	int getShort(void);
};

/**************** 以下 system call ********************************************/

class LcdFuncScreen : public LcdFuncOnlyShort
{
  public:
	LcdFuncScreen(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncOnlyShort(_cmlcd, _mem)
	{
	}

	unsigned long callLcdFunc(void);
};

class LcdFuncGetscreen : public LcdFunc
{
  public:
	LcdFuncGetscreen(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFunc(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncCls : public LcdFuncOnlyShort
{
  public:
	LcdFuncCls(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncOnlyShort(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};


class LcdFuncLcdon : public LcdFuncOnlyShort
{
  public:
	LcdFuncLcdon(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncOnlyShort(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};
class LcdFuncGetlcdon : public LcdFunc
{
  public:
	LcdFuncGetlcdon(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFunc(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};
class LcdFuncMojisize : public LcdFuncOnlyShort
{
  public:
	LcdFuncMojisize(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncOnlyShort(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};


class LcdFuncGetbufsiz : public LcdFuncRect
{
  public:
	LcdFuncGetbufsiz(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncRect(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncReverse : public LcdFuncRect
{
  public:
	LcdFuncReverse(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncRect(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncRevstop : public LcdFuncOnlyShort
{
  public:
	LcdFuncRevstop(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncOnlyShort(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};
class LcdFuncRevstart : public LcdFuncOnlyShort
{
  public:
	LcdFuncRevstart(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncOnlyShort(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};
class LcdFuncRevend : public LcdFuncOnlyShort
{
  public:
	LcdFuncRevend(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncOnlyShort(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncRevdraw : public LcdFunc
{
  public:
	LcdFuncRevdraw(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFunc(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncBlink : public LcdFuncRect
{
  public:
	LcdFuncBlink(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncRect(_cmlcd, _mem)
	{
	}
	int getColor(void)
	{
		return mem->read8_u(10);
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncBlstop : public LcdFuncOnlyShort
{
  public:
	LcdFuncBlstop(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncOnlyShort(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};
class LcdFuncBlstart : public LcdFuncOnlyShort
{
  public:
	LcdFuncBlstart(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncOnlyShort(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};
class LcdFuncBlend : public LcdFuncOnlyShort
{
  public:
	LcdFuncBlend(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncOnlyShort(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncLcdfreeze : public LcdFuncOnlyShort
{
  public:
	LcdFuncLcdfreeze(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncOnlyShort(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncLcdvol : public LcdFuncOnlyShort
{
  public:
	LcdFuncLcdvol(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncOnlyShort(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncGetlcdvol : public LcdFunc
{
  public:
	LcdFuncGetlcdvol(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFunc(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncGv_place : public LcdFuncPoint
{
  private:
	RupAddress *systemAddress;
  public:
	LcdFuncGv_place(CMLcd *_cmlcd, MemoryForInside *_mem, RupAddress *systemAddress) : LcdFuncPoint(_cmlcd, _mem)
	{
		this->systemAddress = systemAddress;
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncGv_kput : public LcdFuncPoint
{
  private:
	LcdFuncMoji moji;

  public:
	LcdFuncGv_kput(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncPoint(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncGv_sput : public LcdFuncPoint
{
  public:
	LcdFuncGv_sput(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncPoint(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncGv_aput : public LcdFuncPoint
{
  public:
	LcdFuncGv_aput(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncPoint(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncGv_line : public LcdFuncRect
{
  public:
	LcdFuncGv_line(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncRect(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};
class LcdFuncGv_square : public LcdFuncRect
{
  public:
	LcdFuncGv_square(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncRect(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncGv_circle : public LcdFuncPoint
{
  public:
	LcdFuncGv_circle(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncPoint(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncGv_pset : public LcdFuncPoint
{
  public:
	LcdFuncGv_pset(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncPoint(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncGv_point : public LcdFuncPoint
{
  public:
	LcdFuncGv_point(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncPoint(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncGv_get : public LcdFuncRect
{
  public:
	LcdFuncGv_get(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncRect(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncGv_put : public LcdFuncPoint
{
  public:
	LcdFuncGv_put(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncPoint(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncGv_scroll : public LcdFuncOnlyShort
{
  public:
	LcdFuncGv_scroll(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncOnlyShort(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncGv_reverse : public LcdFuncRect
{
  public:
	LcdFuncGv_reverse(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncRect(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

class LcdFuncGv_clear : public LcdFuncRect
{
  public:
	LcdFuncGv_clear(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncRect(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};


class LcdFuncGv_kput2 : public LcdFuncPoint
{
  private:
	LcdFuncMoji moji;

  public:
	LcdFuncGv_kput2(CMLcd *_cmlcd, MemoryForInside *_mem) : LcdFuncPoint(_cmlcd, _mem)
	{
	}
	unsigned long callLcdFunc(void);
};

#endif


