// -*-c++-*-
// $Id: state.h,v 1.8 2008/07/26 06:43:08 fumi Exp $

#ifndef STATE_H
#define STATE_H


class RuputerState {
  public:
	enum RuputerState_t {
		RUNNING = 10,
		STOP,
		ENDING_RELOAD,
		ENDING_NEW_LOAD,
		ENDING_DESTROY,
	};

  private:
	enum RuputerState_t state;
	int endingCount;

  public:
	RuputerState(void)
	{
		this->state = RuputerState::STOP;
	}

	void setState(enum RuputerState_t state)
	{
		this->state = state;
	}
	enum RuputerState_t getState(void) const
	{
		return this->state;
	}
	bool isEnding(void) const
	{
		switch(this->state)
		{
		case RuputerState::ENDING_DESTROY:
		case RuputerState::ENDING_NEW_LOAD:
		case RuputerState::ENDING_RELOAD:
			return true;
		case RuputerState::STOP:
		case RuputerState::RUNNING:
			return false;
		}
// not reached
		return false;
	}

	void startEndingCount(void)
	{
		endingCount = 0;
	}
	void incrementEndingCount(void)
	{
		endingCount++;
	}
	bool isEndingCountOver(void)
	{
		if( endingCount >= 64 * 3 ) // 3sec
			return true;
		return false;
	}
};




class ThreadState {
  public:
	enum ThreadState_t {
		WAIT_KEYIN,
		WAIT_BI,
		RUNNING,
		CHILD_RUNNING,

		EXIT,
	};
  private:
	enum ThreadState_t state ;

  public:
	ThreadState(void)
	{
		this->state = ThreadState::EXIT;
	}

	void setState(enum ThreadState_t state)
	{
		this->state = state;
	}
	enum ThreadState_t getState(void) const
	{
		return this->state;
	}
	bool isRunning(void) const
	{
		return state == ThreadState::EXIT ? false : true;
	}
};



#endif
