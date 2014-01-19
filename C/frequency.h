#ifndef __GACB_FREQUENCY__
#define __GACB_FREQUENCY__

	#include <string.h>

	/* get frequency of each char of a text */
	double frequency(const char * const text, double ** f)
	{
		unsigned int i, j;
		double coincidence = 0;
		const char * s = NULL;
		*f = calloc(256, sizeof(double));
		
		/* init the frequency result array */
		for ( i = 0 ; i < 256 ; i++ )
			*(*f+i) = 0;
		
		/* count each possible ASCII char */
		for ( j = 0, s = text ; s != NULL && *s != 0 ; s++, j++ )
			(*(*f+(*s)))++;
		
		/* calculate frequency for each letter, and the index of coincidence */
		for ( i = 0 ; i < 256 ; i++ )
		{
			coincidence += (*(*f+i) * (*(*f+i)-1)) / (j*(j-1));
			*(*f+i) /= j;
		}
		
		return coincidence;
	}

	/* debug infos */
	void show_frequency(const double * const f, const double c)
	{
		size_t i;
		
		for ( i = 0 ; i < 256 ; i++ )
			if ( *(f+i) )
				printf("%d: [%c] -> %f\n", i, i, *(f+i));

		printf("Index of coincidence -> %f\n", c);
	}

#endif
