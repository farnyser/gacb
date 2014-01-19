#ifndef __gacb_encode__
#define __gacb_cencode__
#include <string.h>
#include <math.h>

/*
	Change encoding
*/

void encode(const char * const charset_in, const char * const charset_out, const char * const in, char **out)
{
	if ( strlen(charset_in) > strlen(charset_out) )
		long2short(charset_in, charset_out, in, out);
	else if ( strlen(charset_in) < strlen(charset_out) )
		short2long(charset_in, charset_out, in, out);
	else
	{
		int i = 0;
		*out = calloc(strlen(in)+1, sizeof(char));
		for ( i = 0 ; in[i] != 0 ; i++ )
			(*out)[i] = in[i];
		(*out)[i] = 0;
	}
}

void long2short(const char * const charset_in, const char * const charset_out, const char * const in, char **out)
{
	int i = 0, j = 0;
	int ochar_size = 0;
	int offset = 0;
	
	for ( ochar_size = 0 ; pow(strlen(charset_out), ochar_size) < strlen(charset_in); ochar_size++ );
	
	*out = calloc(strlen(in)*ochar_size+1, sizeof(char));
	
	for ( i = 0 ; in[i] != 0 ; i++ )
	{
		int in_value;
		
		// find in[i] numerical equivalent
		for ( j = 0 ; charset_in[j] != 0 && in[i] != charset_in[j] ; j++ ) {}
		in_value = j;
		
		if ( in_value < strlen(charset_in) )
		{
			for ( j = 0 ; j < ochar_size ; j++ )
			{
				int jj = ochar_size - j - 1;
				int t = floor(in_value / pow(strlen(charset_out), jj)); 
				in_value -= pow(strlen(charset_out), jj) * t;
				(*out)[offset + j] = charset_out[t];
			}
			
			offset += ochar_size;
		}
		else
		{
			(*out)[offset] = in[i];
			offset += 1;
		}
	}
	
	(*out)[i*ochar_size] = 0;
}

void short2long(const char * const charset_in, const char * const charset_out, const char * const in, char **out)
{
	
}



#endif

