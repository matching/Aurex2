// -*-c++-*- 漢字
// $Id: keybuffer.h,v 1.13 2006/02/17 19:01:37 fumi Exp $

#ifndef KEYBUFFER_H
#define KEYBUFFER_H

#define Bkey_dw 0x0080				/* K7端子ビット(十字下1:on,0:off)					*/
#define Bkey_rg 0x0040				/* K6端子ビット(十字右1:on,0:off)					*/
#define Bkey_up 0x0020				/* K5端子ビット(十字上1:on,0:off)					*/
#define Bkey_lf 0x0010				/* K4端子ビット(十字左1:on,0:off)					*/

#define Bkey_B	0x0008				/* K3端子ビット(push(enter) 1:on,0:off)			*/
#define Bkey_C	0x0004				/* K2端子ビット(push(EL)	  1:on,0:off)			*/
#define Bkey_D	0x0002				/* K1端子ビット(push(menu)  1:on,0:off)			*/
#define Bkey_A	0x0001				/* K0端子ビット(push(file)  1:on,0:off)			*/

#define Bkey_EVENT 0x8000

#define KEY_PUSH_DOWN 0x2000
#define KEY_PUSH_UP   0x4000

#define KEY_BUF_SIZE 20

class Register;
class RupAddress;
class RuputerState;

class KeyBuffer
{
  private:
	RupAddress *systemAddress;

	unsigned short buf[KEY_BUF_SIZE];
	int head, tail;
	unsigned short state;

	RuputerState *ruputerState;

  public:
	KeyBuffer(RupAddress *systemAddress, RuputerState *ruputerState);

	void updateState(unsigned short code);
	unsigned short getNowState(void);
	void clearState(void);

	unsigned short enQueueKey(unsigned short code);
	unsigned short deQueueKey(void);
	unsigned short checkQueueKey(void);

	void bsetFirstItem(unsigned short bit);

	void flush(void);

// 1 で regs に変更あり
	int emu_bi_getbtn(Register *regs);
	int emu_Keyin(Register *regs);


	void emu_bi_sensbtn(Register *regs);

};

#endif
