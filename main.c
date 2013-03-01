#include <stdio.h>
#include "dsp.h"

/*
 *	Takes an audio file that can be opened by libsndfile
 *	and does frequency analysis. Or it will when I'm done
 *	with it.
 *	Requires: libsndfile, fftw3
 */

int main(int argc, char **argv)
{
	SNDFILE *sf;
	SF_INFO inf;
	if(argc != 2)
	{
		fprintf(stderr, "USAGE: %s infile", argv[0]);
		return 1;
	}
	sf = sf_open(argv[1], SFM_READ, &inf);
	if(!sf)
	{
		fprintf(stderr, "Input file couldn't be opened.");
		return 1;
	}
	process(sf, &inf);
	return 0;
}