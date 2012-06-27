/* -*-c++-*- */
// $Id: frame.h,v 1.12 2006/02/13 08:56:32 fumi Exp $

#ifndef FRAME_H
#define FRAME_H

class Frame
{
  private:
	int no;
	unsigned long beforeupdate;
	long updateMilliSec;
	unsigned long lasttick;
	long delay;

	unsigned long skipframe;
	unsigned long frame;
	unsigned long renzoku_skipframe;

	static int defaultFrameTime[64];
	int frameTime[64];


	unsigned long start_end_diff;

  private:
	virtual unsigned long getMilliSec(void) = 0;
	virtual void waitMilliSec(unsigned long) = 0;

  public:
	Frame(void);
	virtual ~Frame(void);


	void startFrame(void);
	void endFrame(void);

	int checkTime(void);
	int isFrameUpdateSkip(void);
	void endFrameUpdate(void);

	void wait(void);
	void updateFrameTime(void);

	int getFrameNo(void)
	{
		return no;
	}

	void sync(void);

	int getFrameRateTimes1000(void);
	int getGraphicFrameRateTimes1000(void);

};
#endif
