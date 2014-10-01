#include "stdafx.h"
/* notes
* an 'on' code of 0x0 (null) will represent an epsilon transition or empty
* all the search algorithms are NOT optimized, i wrote this on a plane
*/
#include <iostream>
#include <string>
#include <cstdlib>
#include "nfa_NP.h"

#ifndef ushort
#define ushort unsigned short
#endif

using namespace std;

const int entry = 2;
const int num_states = 8;
const ushort f[8] = { 2, 2, 2, 2, 4, 3, 3, 1 };
const char   o[8] = { 'a', 0x0, 'b', 'a', 'a', 'b', 'b', 0x0 };
const ushort t[8] = { 2, 4, 3, 1, 3, 3, 1, 2 };

void subset(state ** s1, state ** s2, state * ctx, int * size1, int * size2) {
	for (int i = 0; i < *size1; i++)
	{
		if ((*s1)[i].From != ctx->To) continue;

		if ((*s1[i]).On != 0x0)
			/* All non epsilon */
			_set_add_at(s2, s1[i], (*size2)++);
		else
			subset(s1, s2, s1[i], size1, size2);
	}
}

void inference(frag_container* pool, state** s, int size) {
	for (int i = 0; i < size; i++)
	{
		/* search criteria */
		ushort from = (*s)[i].To;
		char on = (*s)[i].On;

		/* return data */
		int set_size = 0;
		subset(s, pool->frags[i].set, s[i], &size, &set_size);
		pool->frags[i].handle = (*s)[i].To;
		pool->frags[i].length = set_size;
	}
	_reduce_frag_spray(pool);
}

/* Void Match( string task, int start_state, frag* fragment_pool ); */
void Match(string s, int n, frag_container pool)
{
	char * cursor = &s[0];
	char * end = &s[s.length()];
	frag ctx = _Get_From_Pool(n, pool);
	RunTimeFragCluster history;

	while (cursor != end)
	{
		
		



	}
	
}

int main(int argc, char const *argv[])
{
	/* set static table */
	size_t length = num_states;
	frag_container pool;
	state** core_ptr = new state*[length];
	state* core = new state[length];

	double hash_id;

	for (int i = 0; i < num_states; i++)
	{
		core[i] = _set_state(f[i], o[i], t[i], &hash_id);
		core_ptr[i] = &(core[i]);
	}
	/* allocate space */
	_set_frag_cont(&pool, length);
	/* create set of all transition results */
	inference(&pool, core_ptr, length);
	/* match string */
	Match( "aa", 2, pool );

	getchar();

	return 0;
}