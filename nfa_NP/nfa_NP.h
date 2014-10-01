#ifndef ushort
#define ushort unsigned short
#endif

using namespace std;
/* 6 bytes */
typedef struct state {
	ushort From;
	char On;
	ushort To;
} state;

typedef struct state_container {
	size_t length;
	state ** states;
} state_container;

/*  16 bytes */
typedef struct  frag {
	ushort handle;
	ushort offset;
	state ** set;
} frag;

typedef struct frag_container {
	size_t length;
	frag * frags;
} frag_container;

typedef struct RunTimeFragCluster {
	size_t begin;
	size_t end;
	bool empty;
	frag_container * cluster;
} RunTimeFragCluster;

inline ostream &operator<<(std::ostream &out, const state &p) {
	out << p.From << ' ' << (p.On == 0x0 ? '*' : p.On ) << ' ' << p.To;
	return out;
}

inline ostream &operator<<(std::ostream &out, const frag &p) {
	out << "From " << p.handle << endl;
	for (size_t i = 0; i < p.offset; i++)
		out << *p.set[i] << endl;
	return out;
}
inline ostream &operator<<(std::ostream &out, const frag_container &p) {
	for (size_t i = 0; i < p.length; i++) {
		for (size_t x = 0; x < p.frags[i].offset; x++)
			out << *(p.frags[i].set[x]) << endl;
		cout << endl;
	}
	return out;
}


void show(state ** set, int length) {
	for (int x = 0; x < length; x++)
		cout << '\t' << *set[x] << endl;
}
void show(frag * set, int length ) {
	for (int x = 0; x < length; x++)
		cout << ' ' << set[x] << endl;
}
state  _set_state(ushort f, char o, ushort t, double * hash) {
	state n;
	n.From = f;
	n.On = o;
	n.To = t;

	*hash = (*hash + f * 3.0f + o * 5.0f + t * 7.0f) / 2;
	return n;
}
void _set_add_at(state ** destination, state * elem, size_t length) {
	for (size_t i = 0; i < length; i++)
		if (destination[i] == elem) return;
	destination[length] = elem;
}

void _set_frag(frag * f, int key, int size, state ** frags)
{
	f->handle = key;
	f->offset = size;
	f->set = frags;
}

void _set_frag_cont(frag_container* c, size_t length) {
	c->frags = new frag[length];
	c->length = length;
	for (size_t i = 0; i < length; i++)
		c->frags[i].set = new state*[length];
}

bool exists(int* b, size_t size, int a) {
	for (size_t i = 0; i < size && size != 0; i++)
		if (b[i] == a) return true;
	return false;
}

void _reduce_frag_spray(frag_container* p) {
	size_t x = 0, size = 0;
	int* temp = new int[p->length];
	for (size_t i = 0; i < p->length; i++) {
		if (!exists(temp, size, p->frags[i].handle )) {
			swap(p->frags[size], p->frags[i]);
			temp[size++] = p->frags[i].handle;
		}
	}
	p->length = size;
}

frag _Get_From_Pool(ushort n, frag_container pool)
{
	for (size_t i = 0; i < pool.length; i++)
		if (pool.frags[i].handle == n)
			return pool.frags[i];
	frag fail;
	fail.handle = 0;
	fail.offset = 0;
	return fail;
}
/****************************************************************************************************/
/****************************************************************************************************/
/****************************************************************************************************/
/* unused */
ushort code(char c) { return c; }
char code(ushort c) { return (char)c; }

void join(state ** destination, state ** source, int * dest_length, int * source_length, int criteria)
{
	int offset = *dest_length;
	bool add = true;
	for (int x = 0; x < *source_length; x++)
	{
		for (int i = 0; i < *dest_length; i++)
		{
			if (destination[i] == source[x] && (*source[x]).From == criteria) add = false;
		}

		if (add)
		{
			destination[offset] = source[x];
			offset++;
			add = false;
		}
	}
	*dest_length = offset;
}
bool contains(state *s1, state *s2)
{
	for (int i = 0; i < sizeof(s1); i++)
	for (int x = 0; x < sizeof(s2); x++)
	if (&s1[i] == &s2[x]) return true;
	return false;
}

int difference(state ** s1, state ** s2, state ** ret, int pos) {
	bool add = true;
	int p = 0;
	for (int i = 0; i < 8; i++) {
		for (int x = 0; x < pos + 1; x++)
		if (s1[i] == s2[x])
			add = false;

		if (add)
		{
			ret[p] = s1[i];
			add = true;
			p++;
		}
	}
	return p;
}


//		2, 2, 2, 2, 4, 3, 3, 1
//		a, *, b, a, a, b, b, *
//		2, 4, 3, 1, 3, 3, 1, 2
