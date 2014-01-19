#include <stdlib.h>
#include <stdio.h>
#include "frequency.h"
#include "string.h" 

int main(int argc, char **argv)
{
	char *input = NULL;
	double *f = NULL, c = 0;
	
	input = gacb_read();
	c = frequency(input, &f);
	show_frequency(f,c);
	
	free(input);
	free(f);
	
	return EXIT_SUCCESS;
}
