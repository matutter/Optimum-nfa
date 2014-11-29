/* notes
* an 'on' code of 0x0 (null) will represent an epsilon transition or empty
* all the search algorithms are NOT optimized, i wrote this on a plane
*/
#include <iostream>
#include <string>
#include <cstdlib>
#include "nfa_NP.hpp"

#ifndef ushort
#define ushort unsigned short
#endif

using namespace std;

const int entry = 2;
const int num_states = 8;
const ushort f[8] = { 2, 2, 2, 2, 4, 3, 3, 1 };
const char   o[8] = { 'a', 0x0, 'b', 'a', 'a', 'b', 'b', 0x0 };
const ushort t[8] = { 2, 4, 3, 1, 3, 3, 1, 2 };

void subset(state ** s1, state ** s2, state * ctx, size_t * size1, size_t * size2) {
	for (size_t i = 0; i < *size1; i++)
	{
		if ((*s1)[i].From != ctx->To) continue;

		if ((*s1[i]).On != 0x0)
			/* All non epsilon */
			_set_add_at(s2, s1[i], (*size2)++);
		else
			subset(s1, s2, s1[i], size1, size2);
	}
}

void inference(frag_container* pool, state** s, size_t size) {
	for (size_t i = 0; i < size; i++)
	{
		/* search criteria */
		ushort from = (*s)[i].To;
		char on = (*s)[i].On;

		/* return data */
		size_t set_size = 0;
		subset(s, pool->frags[i].set, s[i], &size, &set_size);
		pool->frags[i].handle = (*s)[i].To;
		pool->frags[i].length = set_size;
	}
	_reduce_frag_spray(pool);
}

/* Void Match( string task, int start_state, frag* fragment_pool ); */
bool Match( MatchParams p, string s )
{
	char * cursor = &s[0];
	char * end = &s[s.length()];
	context ctx = _CTX_From_Pool(p.start, p.pool);
	RunTimeFragCluster history;
	_RTFC_set( &history, p.size );

	while (cursor != end)
	{
		cout << ctx << endl;
		
		_get_frag_match_on( &ctx, *cursor );
		ctx.ret = cursor;
		if (empty(ctx))
		{
			if (empty(&history))
				return false;
	
			_RTFC_shift(&history, &ctx);
			cursor = ctx.ret;
		
		}
		else
		{
			if (ctx.set.length > 1) 
				_RTFC_append(&history, ctx);
			
			ctx = _CTX_From_Pool(ctx.set.set[ctx.set.length]->To, p.pool);
			
			++cursor;
		}
	}
	return _CTX_satify( ctx, p.accept, p.accept_length );
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
	MatchParams params;
	params.size = length;
	params.start = 2;
	params.accept = new int[1];
	params.accept[0] = 4;
	params.accept_length = 1;
	params.pool = pool;

	string s = "abba";

	cout << (Match(params, s )?"ACCEPT":"FAIL") << ": " << s << endl;

	return 0;
}