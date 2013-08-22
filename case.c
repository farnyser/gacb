#include <stdlib.h>
#include <stdio.h>
#include "string.h" 

int main(int argc, char **argv)
{
	char *input = NULL;
	
	input = gacb_read();
	
	if ( argc > 1 && !strcmp(argv[1], "upper") )
		strtoupper(input);
	else if ( argc > 1 && !strcmp(argv[1], "lower") )
		strtolower(input);
	
	printf("%s", input);
	
	free(input);
	
	return EXIT_SUCCESS;
}
