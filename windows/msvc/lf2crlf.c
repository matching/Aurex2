/* -*-mode:c; coding:shift_jis-dos-*- */

#include <stdio.h>

int lf2crlf(char **argv)
{
	FILE *in, *out;
	int c;

	in  = fopen(argv[0], "rb");
	if( in == NULL )
		return 1;

	out = fopen(argv[1], "wb");
	if( out == NULL )
	{
		fclose( in );
		return 1;
	}

	while((c = getc(in)) != EOF)
	{
		if( c == 0x0a ) /* LF */
			putc(0x0d, out); /* CR */

		putc(c, out);
	}

	fclose( in );
	fclose( out );

	return 0;
}

int main(int argc, char **argv)
{
	argc--;

	if( argc != 2 )
	{
		fprintf(stderr, "Error !!\nusage: %s <infile> <outfile>\n", *argv);
		return 1;
	}

	argv++;

	return lf2crlf(argv);
}



