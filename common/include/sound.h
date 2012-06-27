// -*-c++-*-
// $Id: sound.h,v 1.5 2006/01/28 11:22:31 fumi Exp $

#ifndef SOUND_H
#define SOUND_H

class Ruputer;

class Sound
{
  public:
	static const int UNIT_LENGTH = 345;  // 1/64 ~ 345 / 22050
	static const int FREQUENCY = 22050;
	static const int BIT = 16;

  protected:
	int tempo;
	int volume;

	Ruputer *ruputer;



  public:

	Sound(Ruputer *ruputer, int tempo, int volume);
};


#endif
