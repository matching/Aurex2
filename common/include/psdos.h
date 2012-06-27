/* -*-c++-*- */
// $Id: psdos.h,v 1.12 2006/02/17 19:01:38 fumi Exp $

#ifndef DOS_H
#define DOS_H

class Register;
class Memory;
class Ruputer;
class Directory;
class RuputerDirectory;
class Thread;
class ThreadState;
class X86Reg;

class DosIntr
{
  private:

// System Depend ...
//	SDDosIntr *sidos;


//	RuputerDirectory *dir;
//	SDFile *sdfile;
//	DirectoryMgr *dir;
//	unsigned long psp_address;

	Thread *thread;
	ThreadState *threadState;
	Ruputer *ruputer;
	RuputerDirectory *rupdir;
	Directory *localdir;

  public:
	DosIntr(Ruputer *ruputer, Thread *thread);

	void callDosIntr(Register *regs);

  private:

	void dos_execute(X86Reg *x86reg);
	void dos_child_ret(X86Reg *x86reg);
	void dos_crtmp(X86Reg *x86reg);
	void dos_create(X86Reg *x86reg);
	void dos_crnew(X86Reg *x86reg);
	void dos_open(X86Reg *x86reg);
	void dos_close(X86Reg *x86reg);
	void dos_read(X86Reg *x86reg);
	void dos_write(X86Reg *x86reg);
	void dos_seek(X86Reg *x86reg);
	void dos_delete(X86Reg *x86reg);
	void dos_rename(X86Reg *x86reg);
	void dos_find_next(X86Reg *x86reg);
	void dos_find_first(X86Reg *x86reg);

	void dos_get_psp(X86Reg *x86reg);
	void dos_exit(X86Reg *x86reg, Register *regs);
	void dos_dfspace(X86Reg *x86reg);
	void dos_setdta(X86Reg *x86reg);
	void dos_getdta(X86Reg *x86reg);
	void dos_getcdir(X86Reg *x86reg);
	void dos_chdir(X86Reg *x86reg);
	void dos_getcdrvno(X86Reg *x86reg);
	void dos_gettdrive(X86Reg *x86reg);
	void dos_getcdrive(X86Reg *x86reg);
	void dos_seldrv(X86Reg *x86reg);

	void dos_gettime(X86Reg *x86reg);
	void dos_getdate(X86Reg *x86reg);

	void dos_rmdir(X86Reg *x86reg);
	void dos_mkdir(X86Reg *x86reg);

	unsigned long convToLinearAddress(unsigned short high, unsigned short low);

	int isLocalFileExist(char const *filename);
};

inline unsigned long DosIntr::convToLinearAddress(unsigned short high, unsigned short low)
{
	return (((unsigned long)high<<16) | low) & 0xffffff;
}

#endif
