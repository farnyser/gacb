#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "gacb.h" 

#define POPULATION_SIZE 5000
#define NUMBER_OF_GENERATION 200
#define NUMBER_TO_REPLACE 0.30
#define PROBABILITY_TO_DIE 0.10
#define PROBABILITY_TO_REPRODUCE 0.05
#define PROBABILITY_MUTATION 0.15
#define PROBABILITY_RANDOM_ADD 0.30
#define MAX_AGE 15

int main(int argc, char**argv)
{
	int g;
	gacb_solution generation[2][POPULATION_SIZE];
	gacb_solution best = {NULL,0,0};
	gacb_solution re1 = {NULL,0,0}, re2 = {NULL,0,0};
	double sum;
	
	int i, j, k;
	char *input;
	char charset[] = "azertyuiopqsdfghjklmwxcvbn0123456789";

	/* init random seed */
	srand(time(NULL));
	
	/* reand ciphered text from stdin */
	input = gacb_read();
	
	
	/* init */
	for ( i = 0, g = 0 ; i < POPULATION_SIZE ; i++ )
	{
		generation[g][i].key = gacb_randomkey("azertyuiopqsdfghjklmwxcvbn0123456789",1,10);
		gacb_eval_solution(&(generation[g][i]), charset, input);
		
		generation[g+1][i].key = NULL;
		generation[g+1][i].score = 0;
		generation[g+1][i].age = 0;
		
		gacb_bestify(&best, &(generation[(g)%2][i]));
	}
	
	/* evolv - for X generations */
	for ( g = 0 ; g < NUMBER_OF_GENERATION && best.age < MAX_AGE ; g++ )
	{
		for ( i = 0, sum = 0 ; i < POPULATION_SIZE ; i++ )
		{
			sum += generation[g%2][i].score;
			
			if ( generation[g%2][i].key != NULL )
			{
				generation[(g+1)%2][i].key = gacb_text(generation[g%2][i].key);
				generation[(g+1)%2][i].score = generation[g%2][i].score;
				generation[(g+1)%2][i].age = generation[g%2][i].score+1;
				
				if (((rand()/(float)RAND_MAX)) < PROBABILITY_MUTATION )
				{
					gacb_mutate("azertyuiopqsdfghjklmwxcvbn0123456789",generation[(g+1)%2][i].key);
					gacb_eval_solution(&(generation[(g+1)%2][i]), charset, input);
					generation[(g+1)%2][i].age = 0;
					
					printf("[mutate] %s(%f)->%s(%f)\n", generation[(g)%2][i].key, generation[(g)%2][i].score, generation[(g+1)%2][i].key, generation[(g+1)%2][i].score);
				
					gacb_bestify(&best, &(generation[(g+1)%2][i]));
				}
			}
		}
		best.age++;
		gacb_sort((generation[g%2]), 0, POPULATION_SIZE-1);
		
		printf("Generation %d, sum of score -> %f\n", g, sum);
		
		for ( k = 0, i = POPULATION_SIZE-1 ; k < (NUMBER_TO_REPLACE*POPULATION_SIZE) ; i-- )
		{
			/* loop */
			if ( i < 0 ) 
				i = POPULATION_SIZE-1;
			
			/* die */
			if ( generation[(g)%2][i].age > MAX_AGE || ((rand()/(float)RAND_MAX)) < PROBABILITY_TO_DIE )
			{
				if ( ((rand()/(float)RAND_MAX)) < PROBABILITY_RANDOM_ADD )
				{
					generation[(g+1)%2][i].key = gacb_randomkey("azertyuiopqsdfghjklmwxcvbn0123456789",1,10);
					printf("[random add] %s\n", generation[(g+1)%2][i].key);
					gacb_eval_solution(&(generation[(g+1)%2][i]), charset, input);
					generation[(g+1)%2][i].age = 0;
						
					gacb_bestify(&best, &(generation[(g+1)%2][i]));
				}
				else
				{
					/* select 2 reproducers */
					re1.key = NULL;
					re1.score = 0;
					re2.key = NULL;
					re2.score = 0;
					
					while ( re1.key == NULL || re2.key == NULL )
					{
						for ( j = 0 ; ; j++ )
						{
							/* loop */
							if ( j >= POPULATION_SIZE ) 
								j = 0;
							
							if ( ((rand()/(float)RAND_MAX)) < PROBABILITY_TO_REPRODUCE )
							{
								if ( re1.key == NULL )
								{
									re1.key = gacb_text(generation[g%2][j].key);
									re1.score = generation[g%2][j].score;
									break;
								}
								else if ( re2.key == NULL && strcmp(generation[g%2][j].key, re1.key) )
								{
									re2.key = gacb_text(generation[g%2][j].key);
									re2.score = generation[g%2][j].score;
									break;
								}
							}

							if ( re1.key != NULL && re2.key != NULL )
								break;
						}
					}
				
			
					/* two good */
					if ( re1.key != NULL && re2.key != NULL )
					{
						generation[(g+1)%2][i].key = gacb_merge(re1.key,re2.key);
						printf("[merge] -> %s %s => %s \n", re1.key, re2.key, generation[(g+1)%2][i].key);
						free(re1.key); re1.key = NULL;
						free(re2.key); re2.key = NULL;
						gacb_eval_solution(&(generation[(g+1)%2][i]), charset, input);
						
						gacb_bestify(&best, &(generation[(g+1)%2][i]));
					}
				
				}
				
				k++;
				i=-1; /* restart from end */
			}
		}
		
		/* clean old generation */
		for ( i = 0 ; i < POPULATION_SIZE ; i++ )
		{
			if ( generation[g%2][i].key != NULL )
				free(generation[g%2][i].key);
			
			generation[g%2][i].key = NULL;
			generation[g%2][i].score = -1;
			generation[g%2][i].age = -1;
		}
		
		printf("Generation %d, best -> %f (%s)\n", g, best.score, best.key);
	}
	
	//~ gacb_sort((generation[g%2]), 0, POPULATION_SIZE-1);
	
	/* clean currrent generation */
	for ( i = 0 ; i < POPULATION_SIZE ; i++ )
	{
		//~ if ( i < 100 )
		//~ printf("[%d] %f (%s)\n",i,generation[g%2][i].score,generation[g%2][i].key);
		
		if ( generation[g%2][i].key != NULL )
			free(generation[g%2][i].key);
		
		generation[g%2][i].key = NULL;
		generation[g%2][i].age = -1;
		generation[g%2][i].score = -1;
	}
	
	printf("%s (%f)\n", best.key, best.score);

	
	return EXIT_SUCCESS;
}
