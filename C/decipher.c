#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "cipher.h"

void help (void)
{
	printf("Usage: decipher alphabet key\n");
}

int main(int argc, char **argv)
{
	char *input = NULL;
	char *output = NULL;

	if ( argc != 3 ) 
	{
		help();
		return EXIT_FAILURE;
	}
	
	input = gacb_read();
	decipher(argv[1], argv[2], input, &output);
	printf("%s", output);

	free(output);
	free(input);

	return EXIT_SUCCESS;
}
