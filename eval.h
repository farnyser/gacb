#ifndef __GACB_EVAL__
#define __GACB_EVAL__
#include <stdio.h>
#include "frequency.h"

double gacb_abs(double x) 
{
	if ( x < 0 )
		return -x;
	else
		return x;
}
double gacb_max(double x,double y) 
{
	if ( x < y )
		return y;
	else
		return x;
}

/* evaluate a text for a specified language */
double gacb_eval(const char * const text, const double f[], const double c, const char * words[])
{
	/* result */
	double r = 0;
	unsigned int ponderation = 0;
	
	/* temproary variables */
	double *ft;
	size_t *wi;
	double ct = 0;
	size_t i, j;

	/* analyse text */
	ct = frequency(text,&ft);
	
	/* char frequency */
	for ( i = 0 ; i < 256 ; i++ )
	{
		if ( f[i] )
		{
			r += 1 - gacb_abs(f[i]-ft[i])/gacb_max(f[i],ft[i]);
			ponderation++;
		}
	}
	
	/* init for word search */
	for ( i = 0 ; words[i] != NULL ; i++ ) {}
	wi = malloc(i * sizeof(size_t));
	for ( i = 0 ; words[i] != NULL ; i++ ) { wi[i] = 0; }
	
	/* word search - works well, but very slow */
	for ( i = 0 ; text[i] != 0 ; i++ )
	{
		for ( j = 0 ; words[j] != NULL ; j++ )
		{
			if ( words[j][wi[j]] != text[i] )
			{
				wi[j] = (int)(words[j][0] == text[i]);					
			}
			else
			{
				wi[j]++;
				
				if ( words[j][wi[j]] == 0 )
				{
					r++;
					ponderation++;
					wi[j] = 0;
				}
			}
			
		}
	}
	
	/* index of coincidence */
	r += (1 - gacb_abs(c-ct)/gacb_max(c,ct))*ponderation;
	ponderation *= 2;
	
	/* average */
	r/=ponderation;
	
	return r;
}

#endif /* __GACB_EVAL__ */

