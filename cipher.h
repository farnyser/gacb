#ifndef __gacb_cipher__
#define __gacb_cipher__
#include <string.h>

/*
	Vigenere Cipher
*/

/* cipher or decipher */
typedef enum {vigenere_cipher=1, vigenere_decipher=-1} vigenere_action;

/* main algorithm */
void vigenere(const vigenere_action action, const char * const charset, const char * const key, const char * const in, char **out)
{
	/* iterators */
	size_t i, j, k;

	/* init some constants */
	const size_t charset_size = strlen(charset);
	const size_t key_size = strlen(key);
	const size_t text_size = strlen(in);

	/* allocate memory for the result */
	*out = calloc(text_size+1, sizeof(char));

	/* foreach char of the input text */
	for ( i = 0 ; in[i] != 0 ; i++ )
	{
		/* get position in charset for text[i] */
		for ( j = 0 ; j < charset_size && charset[j] != in[i] ; j++ ) {}
		
		if ( j < charset_size )
		{
			/* calculate new position */
			for ( k = 0 ; charset[k] != key[i%key_size] && k < charset_size ; k++ ) {}
			/* write output */
			(*out)[i] = charset[(charset_size+j+k*action)%charset_size]; 
		}
		else
		{
			/* write output */
			(*out)[i] = in[i]; 
		}
	}
	
	(*out)[i] = 0;
}

/* cipher shortcut */
void cipher(const char * const charset, const char * const key, const char * const in, char **out)
{
	vigenere(vigenere_cipher, charset, key, in, out);
}

/* decipher shortcut */
void decipher(const char * const charset, const char * const key, const char * const in, char **out)
{
	vigenere(vigenere_decipher, charset, key, in, out);
}

#endif

