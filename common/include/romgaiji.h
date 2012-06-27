// -*-c++-*-
// $Id: romgaiji.h,v 1.2 2004/04/29 09:44:32 fumi Exp $

#ifndef ROMGAIJI_H
#define ROMGAIJI_H

struct GaijiData
{
	unsigned short code;
	unsigned char data[24];
};

class RomGaiji
{
	static GaijiData gaijidata[];
	int ngaiji;

  public:
	RomGaiji(void);
	~RomGaiji(void);
	bool isGaiji(unsigned short code);
	unsigned char *getGaijiData(unsigned short code);
};

#endif
