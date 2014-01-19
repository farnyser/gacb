#ifndef __gacb_string__
#define __gacb_string__

#include <string.h>

/* read data and create a buffer to store it */
char* gacb_read()
{
	size_t dsize = 80;
	size_t i = 0;
	char *text = calloc(dsize,sizeof(char));
	char *tmp = NULL;
	
	/* get input from stdin */
	while ( (*(text+i) = fgetc(stdin)) != EOF ) 
	{
		i++;
		
		if ( i >= dsize ) 
		{
			dsize *= 2;
			tmp = realloc(text,dsize);
			
			if ( tmp == NULL )
			{
				free(text);
				return NULL;
			}
			else
			{
				text = tmp;
			}
		}
	}
	
	/* end of string */
	*(text+i) = 0;
	
	/* free not-needed memory */
	if ( (text = realloc(text,i)) == NULL ) {}
	
	return text;
}

/* create a buffer to store data */
char* gacb_text(const char data[])
{
	size_t i;
	char *text = malloc((strlen(data)+1) * sizeof(char));
	if ( text != NULL )
	{
		for ( i = 0 ; data[i] != 0 ; i++ ) { text[i] = data[i]; }
		text[i] = 0;
	}
	return text;
}

/* to lower (in place) */
void strtolower(char *data)
{
	char *str = NULL;
	
	for ( str = data; *str != 0 ; str++ )
		if ( *str >= 'A' && *str <= 'Z' )
			*str += 'a'-'A';
}

/* to upper (in place) */
void strtoupper(char *data)
{
	char *str = NULL;
	
	for ( str = data; *str != 0 ; str++ )
		if ( *str >= 'a' && *str <= 'z' )
			*str += 'A'-'a';
}

#endif /* __gacb_string__ */
