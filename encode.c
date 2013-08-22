#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "encode.h"

void help (void)
{
	printf("Usage: encode alphabet new_alphabet\n");
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
	encode(argv[1], argv[2], input, &output);
	printf("%s", output);

	free(output);
	free(input);

	return EXIT_SUCCESS;
}
