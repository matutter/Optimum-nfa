/*
	This is computationally terrible and resource abusive...
	I just did this to finish some homework for me.
*/

#include <iostream>
#include "dfaMinimizer.hpp"
#include <vector>
#include <map>


//#define DEBUG
#define SET vector<State>
#define MULTISET vector<vector<State> >

using namespace std;

bool samePartition( MULTISET, int, int );
bool sameSet( SET, int, int );
void removeExcess( SET& );
SET  regroup( MULTISET& );
void reduce( MULTISET& );

int main(int argc, char const *argv[])
{

	/* create the table */
	SET s = exampleTable2();
	/* remove states that aren't the start state and nothing transitions to */
	removeExcess( s );
	cout << "\n\nWithout impossible producers" << endl;
	print("\n\tUSED\ta\tb", s);


	/* split into accept, and not accept states */
	SET Accept = acceptStates( s );
	SET Normal = normalStates( s );
	cout << "\n\nInitial partition" << endl;
	print("\n\tAccpt\ta\tb", Accept);
	print("\n\tNorml\ta\tb", Normal);

	/* put those into a multiset */
	MULTISET master;
	master.push_back(Accept);
	master.push_back(Normal);

	reduce( master );

	cout << "\n\nReduced partitions" << endl;
	for (int i = 0; i < master.size(); ++i)
		print("",master[i]);
	
	SET final = regroup( master );
	print("\tFINAL",final);

	cout << endl << endl;
	return 0;
		
}


bool checkOrSplit( MULTISET &v, int i ) {
	bool mismatch = false;
	State check1 = v[i][0];
	State check2 = v[i][1];

	for (int c = 0; c < SIZE_OF_ALPHABET; ++c) {
		int v1 = check1.On( Alphabet[c] );
		int v2 = check2.On( Alphabet[c] );
		if( v1 == v2 ) continue;
		if( samePartition( v, v1, v2 ) ) continue;
		mismatch = true;
		break;
	}
	if( !mismatch ) return true;

	v.erase( v.begin() + i );

	SET s1, s2;
	s1.push_back(check1);
	s2.push_back(check2);

	v.push_back(s1);
	v.push_back(s2);

	return false;
}

void reduce( MULTISET &v ) {
	/* for each partition */
	for (int i = 0; i < v.size(); ++i) {
		if( v[i].size() < 2 ) continue;
		else {
			if( v[i].size() == 2 ) {
				if( checkOrSplit( v, i ) )
					continue;
				else 
					reduce(v);
			}
		}
		/* check if each partion's elements */
		for (int j = 0; j < v[i].size(); ++j) {
			/* against other elements in that partition */
			for (int y = j; y < v[i].size(); ++y) {
				if( v[i][j] == v[i][y] ) continue;
				/* if they transition on the same characters */
				bool okay = true;
				for (int c = 0; c < SIZE_OF_ALPHABET && okay; ++c) {
					/* with edges that end in the same partition */
					int v1 = v[i][j].On( Alphabet[c] );
					int v2 = v[i][y].On( Alphabet[c] );
					if( v1 == v2 ) continue; /* same vertex is clearly in the same partition */
					#ifdef DEBUG
						cout << "is " << v1 << "," << v2 << " in the same partition?" << endl;
					#endif
					if( !samePartition( v, v1, v2 ) ) okay = false;
				}
				if( okay ) {
					/* will both become a new partion */
					SET v2;
					v2.push_back( v[i][j] );
					v2.push_back( v[i][y] );
					#ifdef DEBUG
						for (int k = 0; k < v.size(); ++k)
							print("B",v[k]);
						cout << "REMOVE " << v[i][j] << endl;
						cout << "REMOVE " << v[i][y] << endl;
					#endif
					v[i].erase( v[i].begin() + j );					
					v[i].erase( v[i].begin() + y-1 );
					v.push_back( v2 );				
					#ifdef DEBUG
						for (int k = 0; k < v.size(); ++k)
							print("A",v[k]);
					#endif
					reduce( v );
					return;
				}				
			}
		}
	}
}

bool samePartition( MULTISET v, int a, int b ) {
	int size = v.size();
	for (int i = 0; i < size; ++i)
		if( sameSet(v[i], a, b) ) return true;
	return false;
}

bool sameSet( SET v, int a, int b ) {
	int found = 0;
	for (int i = 0; i < v.size() && found < 2; ++i)
		if( v[i].vertex == a || v[i].vertex == b )
			found++;
	return found == 2;
}

SET regroup( MULTISET &v ) {
	typedef map<int, int> pairwise;
	pairwise pair;
	SET completed;
	int id = 1;
	cout << endl;

	/* initialize ID map */
	for (int i = 0; i < v.size(); ++i, ++id) {
		for (int j = 0; j < v[i].size(); ++j) {
			if( pair.find( v[i][j].vertex ) == pair.end() ){
				pair[ v[i][j].vertex ] = id;
				#ifdef DEBUG
					cout << "\tMap(" << v[i][j].vertex  << ',' << id << ")\n";
				#endif
			}
		}
	}
	cout << endl;

	for (int i = 0; i < v.size(); ++i) {
		completed.push_back( State(false) );
		/* assign ID */
		id = pair[ v[i].back().vertex ];
		completed.back().edges = new Transition[SIZE_OF_ALPHABET];

		/* for each symbol */
		for( int c = 0; c < SIZE_OF_ALPHABET; ++c ) {
			char on = Alphabet[c];
			bool unset = true;
			/* check each state */
			for (int j = 0; j < v[i].size(); ++j) {
				/* combine the types of each into one */
				if( v[i][j].Accept ) completed.back().Accept = true;
				if( v[i][j].Start  ) completed.back().Start  = true;	

				/* find the first successful transition */
				int  to = v[i][j].On( on );
				if( to == 0 ) continue;
				unset = false;
				completed.back().Set( id, c, pair[to] );
			}
			/* if there is no proper transition, allow it to move to state 0*/
			if( unset )
				completed.back().Set( id, c, 0x0 );
		}
	}	

	return completed;
}

void removeExcess( SET& v ) {
	for (int i = 0; i < v.size(); ++i) {
		if( v[i].Start ) continue;
		int  vert = v[i].vertex;
		bool okay = false;
		for (int j = 0; j < v.size(); ++j) {
			for (int c = 0; c < SIZE_OF_ALPHABET && !okay; ++c) {
				if( v[j].On( Alphabet[c] ) == vert ) {
					okay = true;
					break;
				}	
			}
		}
		if( !okay ) {
			v.erase( v.begin() + i );
		}
	}

}