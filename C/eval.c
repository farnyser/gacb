#include <stdlib.h>
#include <stdio.h>
#include "eval.h"
#include "string.h" 

int main(int argc, char **argv)
{
	int i;
	char *input = NULL;
	double result;
	
	/* french text */
	const double french_index = 0.778;
	const char* french_words[28];
	double french_frequency[256];
	for ( i = 0 ; i < 28 ; i++ ) 
		french_words[i] = NULL;
	for ( i = 0 ; i < 256 ; i++ ) 
		french_frequency[i] = 0;
	
	french_frequency['e'] = 0.1587;
	french_frequency['a'] = 0.0942;
	french_frequency['i'] = 0.0841;
	french_frequency['s'] = 0.0790;
	french_frequency['t'] = 0.0726;
	french_frequency['n'] = 0.0715;
	french_frequency['r'] = 0.0646;
	french_frequency['u'] = 0.0624;
	french_frequency['l'] = 0.0534;
	french_frequency['o'] = 0.0514;
	french_frequency['d'] = 0.0339;
	french_frequency['m'] = 0.0324;
	french_frequency['p'] = 0.0286;
	french_frequency['c'] = 0.0264;
	french_frequency['v'] = 0.0215;
	french_frequency['q'] = 0.0106;
	french_frequency['g'] = 0.0104;
	french_frequency['b'] = 0.0102;
	french_frequency['f'] = 0.0095;
	french_frequency['j'] = 0.0089;
	french_frequency['h'] = 0.0077;
	french_frequency['z'] = 0.0032;
	french_frequency['x'] = 0.0030;
	french_frequency['y'] = 0.0024;
	french_frequency['k'] = 0.0000;
	french_frequency['w'] = 0.0000;
	
	french_words[0] = gacb_text("avec");
	french_words[1] = gacb_text("tout");
	french_words[2] = gacb_text("grand");
	french_words[3] = gacb_text("petit");
	french_words[4] = gacb_text("main");
	french_words[5] = gacb_text("plus");
	french_words[6] = gacb_text("faire");
	french_words[7] = gacb_text("dans");
	french_words[8] = gacb_text("elle");
	french_words[9] = gacb_text("pour");
	french_words[10] = gacb_text("vous");
	french_words[11] = gacb_text("dire");
	french_words[12] = gacb_text("nous");
	french_words[13] = gacb_text("comme");
	french_words[14] = gacb_text("mais");
	french_words[15] = gacb_text("aide");
	french_words[16] = gacb_text("chef");
	french_words[17] = gacb_text("droit");
	french_words[18] = gacb_text("gauche");
	french_words[19] = gacb_text("mort");
	french_words[20] = gacb_text("four");
	french_words[21] = gacb_text("fois");
	french_words[22] = gacb_text("madame");
	french_words[23] = gacb_text("carte");
	french_words[24] = gacb_text("cause");
	french_words[25] = gacb_text("chose");
	french_words[26] = gacb_text("classe");
	
	/* main functions */
	input = gacb_read();
	
	/*for ( i = 0 ; i < 100000 ; i++ )*/
	result = gacb_eval(input,french_frequency,french_index,french_words);
	
	printf("%f\n", result);
	
	free(input);
	
	return EXIT_SUCCESS;
}
