#ifndef __GACB__
#define __GACB__

	#include <string.h>
	#include "string.h"
	#include "eval.h"
	#include "cipher.h"
	#include "frequency.h"

	typedef char* gacb_key;
	typedef float gacb_score;
	typedef int gacb_age;
	typedef struct {
		gacb_key key;
		gacb_score score;
		gacb_age age;
	} gacb_solution;
	
	void gacb_eval_solution(gacb_solution *s, const char * const charset, const char * const text)
	{
		char *tmp;
		
			/* french text */
			int i;
			const double french_index = 0.778;
			const char* french_words[16];
			double french_frequency[256];
			for ( i = 0 ; i < 16 ; i++ ) 
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
		
		decipher(charset, s->key, text, &tmp);
		s->score = gacb_eval(tmp,french_frequency,french_index,french_words);
		free(tmp);
	}
	
	
	void gacb_bestify(gacb_solution *best, gacb_solution *s)
	{
		if ( s->score > best->score )
		{
			if ( best->key != NULL ) 
			{
				free(best->key); 
				best->key = NULL;
			}
			
			best->score = s->score;
			best->key = gacb_text(s->key);
			best->age = 0;
			printf("new best: %f %s\n", best->score, best->key);
		}					
	}
	
	
	void gacb_mutate(const char * const charset, char *k)
	{
		/* substitution */
		k[(int)((rand()/(float)RAND_MAX) * strlen(k))] = charset[(int)((rand()/(float)RAND_MAX) * strlen(charset))];
	}
	
	char * gacb_randomkey(const char * const charset, const size_t min, const size_t max)
	{
		size_t size, i;
		char *r = NULL;
		
		/* constant */
		const size_t charset_size = strlen(charset);
		
		/* size of the key */
		size = min + (int)(rand()/(float)RAND_MAX * max);
		r = malloc(sizeof(char) * (size+1));
		
		if ( r != NULL )
		{
			/* random each char */
			for ( i = 0 ; i < size ; i++ ) 
			{
				r[i] = charset[(int)(rand()/(float)RAND_MAX * charset_size)];
			}	
			r[i] = 0;
		}
		
		return r;
	}
	
	//~ char * gacb_merge(const char * const k1, const char * const k2)
	//~ {
		//~ /* iterator */
		//~ size_t i;
		
		//~ /* constant */
		//~ const size_t k1_s = strlen(k1);
		//~ const size_t k2_s = strlen(k2);

		//~ /* cut at */
		//~ size_t k1_o = (int)(rand()/(float)RAND_MAX * (k1_s-1));
		//~ size_t k1_oo = 1+k1_o+(int)(rand()/(float)RAND_MAX * (k1_s-k1_o-1));
		//~ size_t k2_o = (int)(rand()/(float)RAND_MAX * (k2_s-1));
		//~ size_t k2_oo = 1+k2_o+ (int)(rand()/(float)RAND_MAX * (k2_s-k2_o-1));
				
		//~ /* result */
		//~ char *r = malloc((k1_o+(k1_s-k1_oo)+(k2_oo-k2_o)+1) * sizeof(char));
		
		
		//~ if ( r != NULL )
		//~ {
			//~ /* copy each char, 0 terminated */
			//~ for ( i = 0 ; i < k1_o ; i++ ) r[i] = k1[i];
			//~ for ( i = k2_o ; i < k2_oo ; i++ ) r[k1_o+i-k2_o] = k2[i];
			//~ for ( i = k1_oo ; i < k1_s ; i++ ) r[k1_o+(k2_oo-k2_o)+-k1_oo+i] = k1[i];
			//~ r[k1_o+(k1_s-k1_oo)+(k2_oo-k2_o)] = 0;
		//~ }
		
		//~ return r;
	//~ } 
	
	char* gacb_merge(const char * const k1, const char * const k2)
	{
		/* iterator */
		size_t i;
		
		/* constant */
		const size_t k1_s = strlen(k1);
		const size_t k2_s = strlen(k2);
		const k_max = k1_s > k2_s ? k1_s : k2_s;
		
		/* cut at */
		size_t k1_o = (int)(rand()/(float)RAND_MAX * (k1_s-1));
		double random = 0;
		
		/* result */
		char *r = malloc((k_max+1) * sizeof(char));
		
		
		if ( r != NULL )
		{
			/* copy each char, 0 terminated */
			for ( i = 0 ; i < k_max ; i++ )
			{
				random = rand()/(float)RAND_MAX;
				
				if ( random < 0.49 )
				{
					if ( i < k1_s )
						r[i] = k1[i];
					else
					{
						r[i] = 0;
						break;
					}
				}
				else if ( random < 0.98 )
				{
					if ( i < k2_s )
						r[i] = k2[i];
					else
					{
						r[i] = 0;
						break;
					}
				}
				else if ( i > 0 )
				{
					r[i] = 0;
					break;
				}
				else
				{
					i--;
				}
			}
			
			r[i] = 0;
		}
		
		return r;
	}
	
	//~ char * gacb_merge(const char * const k1, const char * const k2)
	//~ {
		//~ /* iterator */
		//~ size_t i;
		
		//~ /* constant */
		//~ const size_t k1_s = strlen(k1);
		//~ const size_t k2_s = strlen(k2);

		//~ /* cut at */
		//~ size_t k1_o = 1+(int)(rand()/(float)RAND_MAX * (k1_s-1));
		//~ size_t k2_o = 1+(int)(rand()/(float)RAND_MAX * (k2_s-1));
		
		//~ /* result */
		//~ char *r = malloc((k1_o+(k2_s-k2_o)+1) * sizeof(char));
		
		//~ if ( r != NULL )
		//~ {
			//~ /* copy each char, 0 terminated */
			//~ for ( i = 0 ; i < k1_o ; i++ ) r[i] = k1[i];
			//~ for ( i = k2_o ; i < k2_s ; i++ ) r[k1_o+i-k2_o] = k2[i];
			//~ r[k1_o+(k2_s-k2_o)] = 0;
		//~ }
		
		//~ return r;
	//~ }
	
	int gacb_partsort(gacb_solution *t, int p, int r) 
	{
		float pivot = t[p].score;
		int	i = p-1, j = r+1;
		gacb_solution temp;
		while (1) {
			do
				j--;
			while (t[j].score < pivot);
			do
				i++;
			while (t[i].score > pivot);
			if (i < j) {
				temp = t[i];
				t[i] = t[j];
				t[j] = temp;
			}
			else
				return j;
		}
	}
	
	void gacb_sort(gacb_solution *t, int p, int r)
	{
		int q;
		
		if (p < r) 
		{
			q = gacb_partsort(t, p, r);
			gacb_sort(t, p, q);
			gacb_sort(t, q+1, r);
		}		
	}

#endif /* __GACB__ */

