// -*-c++-*-
// $Id: aapixel.h,v 1.2 2005/01/06 14:21:33 fumi Exp $

#ifndef AAPIXEL_H
#define AAPIXEL_H

class AAPixel
{
  private:
	unsigned char *pixel;

	int width, height;
	int offset_top, offset_left;
	int all_width, all_height;

  public:
	AAPixel(int zoom);
	~AAPixel(void);

	int getWidth(void);
	int getHeight(void);
	int getAllWidth(void);
	int getAllHeight(void);
	int getOffsetTop(void);
	int getOffsetLeft(void);

	unsigned char const *getPixelLine(int y);

  private:
	void setPixelData(int zoom);
	unsigned char getPixelValue(int x, int y, int zoom);
};


#endif
