/* $Id: parse.h,v 1.2 2004/05/03 18:00:49 fumi Exp $ */
#ifndef PARSE_H
#define PARSE_H

class BdfFontSet;

int c_parse(BdfFontSet *bdffont, char const *filename, int height);
void kakuno(unsigned short code, int width, char *buf);

#endif
