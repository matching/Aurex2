/* -*-c++-*-
 * $Id: ruputer.h,v 1.48 2008/07/26 06:43:08 fumi Exp $
 */

#ifndef RUPUTER_H
#define RUPUTER_H

#include "exffile.h"
#include "state.h"

#include <list>
using namespace std;

class MemoryForInside;
class MemoryForCpu;
class Frame;
class TimerMgr;
class SDDirectory;
class KeyBuffer;
class Threadable;
class MainThread;
class ChildThread;

class SDLcd;
class CMLcd;

class WatchDT;
class SysTimer;
class Directory;
class SDSound;

class Option;
class FilerIniMgr;
class SDFindFile;

class RuputerDirectory;
class RupAddress;

class UndefinedInstructionException;
class OddAccessException;
class UFilerIni;

class Ruputer
{
  protected:
	FilerIniMgr *filerini;
	ExfFile *exffile;
	KeyBuffer *keybuf;

  private:
/*
	MemoryForInside *mem;
*/
	MemoryForCpu *memoryForCpu;

	SysTimer *systimer;
	WatchDT *watchdt;

	int cpuspeed;
	int framespeed;

	unsigned long psp_address;
	unsigned long psp_address_save;
	unsigned long dta_address;

	TimerMgr *timermgr;
	TimerMgr *systimermgr;
//	ThreadMgr *threadmgr;
	list<Threadable *> threadList;

	SDSound *sdsound;

	Directory *localDirectory;
	RuputerDirectory *ruputerDirectory;

	RupAddress *systemAddress;

	CMLcd *cmlcd;
	SDLcd *sdlcd;


	RuputerState ruputerState;
	ThreadState mainThreadState;
	MainThread *mainThread;
	char *newLoadFileName;

	ChildThread *childThread;
	int childExitCode;

  public:
	Frame *frame;

  public:
	Ruputer(Frame *frame, Directory *sddir, Option *option, UFilerIni *user_ini);
	virtual ~Ruputer(void);

	RupAddress *getSystemAddress(void);

	list<Threadable *> *getThreadList(void);
	KeyBuffer *getKeyBuffer(void);

	unsigned long getPspAddress(void);
	void setPspAddress(unsigned long psp);

	unsigned long getDtaAddress(void);
	void setDtaAddress(unsigned long dta);

	SDSound *getSDSound(void);
	void setSDSound(SDSound *sdsound);

	RuputerDirectory *getRuputerDirectory(void);
	Directory *getLocalDirectory(void);
	MemoryForCpu *getMemoryForCpu(void);

	RuputerState *getState(void);
	TimerMgr *getTimerMgr(void);
	TimerMgr *getSysTimerMgr(void);
	WatchDT *getWatchDT(void);

	void deleteMainThread(void);
	void requestDestroy(void);
	virtual void destory(void);

	void requestReload(void);

	void requestNewLoad(char const *fileName);




#if 0
	int loadOption(Directory *dir=NULL);
	int loadFilerIni(Directory *dir=NULL);
#endif

	int main_loop(void);

	void setSDLcd(SDLcd *sdlcd);
	SDLcd *getSDLcd(void);
	CMLcd *getCMLcd(void);

	virtual int load(char const *filename) throw(ExfFileInvalidException, ExfFileNotFoundException, AssociatedFileNotInVirtualDirectoryException);

	void OptionIsChanged(Option *options);
	void init(void);
	void closeAllThread(void);

	virtual SDFindFile *createSDFindFile(struct dta_t *dta_area, char const *pathname, unsigned char search_attribute) = 0;


	void reload(void);
	virtual void setupLog(void);

	void loadChild( char const *filename, char *newargs );
	int  getChildExitCode(void);

  private:
	int getExecutableFile(char const *filename, char *exf) throw(ExfFileNotFoundException);

	void updateFrame(void);

	virtual int EventProcedure(void)=0;
	int interrupt64Hz(void);
	int interrupt4Hz(void);
	int interrupt1Hz(void);

// error message
	virtual bool printErrorMessageForUndefinedInstruction(UndefinedInstructionException *e);
	virtual bool printErrorMessageForOddAccess(OddAccessException *e);
};

// extern Ruputer *ruputer;

#endif
