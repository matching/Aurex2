/* $Id: itoa.h,v 1.1 2004/04/29 09:44:33 fumi Exp $
 */

#ifndef ITOA_H
#define ITOA_H

char *util_uitoa_supply_char(unsigned int val, char *str, int cols, int pad);
char *util_uitoa(unsigned int val, char *str);

#endif
