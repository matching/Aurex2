#ifndef SOFTKEY_H
#define SOFTKEY_H

struct softkey_mmp_info
{
	int start_x;
	int width;
	int col;
};

#define KEYTBL_Y_OFFSET 28

unsigned short smallen_process(int key, int *mode);

#endif
