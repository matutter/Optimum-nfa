#define SIZE_OF_ALPHABET 2

#include <iostream>
#include <vector>
using namespace std;
const char Alphabet[SIZE_OF_ALPHABET] = {'a','b'};
const size_t table_size = 8;




class Transition {
public:
	char on;
	int  to;
	Transition() {}
	void Set(char o, int t) {
		this->on = o;
		this->to = t;
	}
};

class State {
public:
	int vertex, i;
	bool skip, Accept, Start;
	Transition * edges;

	State(){
		this->i = 0;
		this->skip = this->Accept = this->Start = false;
		this->edges = new Transition[SIZE_OF_ALPHABET];
	}
	State(bool f) {
		this->i = 0;
		this->skip = this->Accept = this->Start = false;		
	}
	void Set(int id, char c, int i){
		this->vertex = id;
		this->edges[this->i] = Transition();
		this->edges[this->i].Set(c,i);
		this->i++;
	}
	void Set(int id, int c, int i){
		this->vertex = id;
		this->edges[c].Set(c,i);
	}
	int On( char c ) {
		for (int i = 0; i < this->i; ++i)
			if(this->edges[i].on == c)
				return this->edges[i].to;
		return 0;
	}
	std::string type() {
		std::string text = "  ";
		text[0] = this->Accept? '*' : ' ';
		text[1] = this->Start? '>' : ' ';
		return text;	
	}
	bool operator == (const State &ref) const 
	{
		if( this->vertex == ref.vertex ) return true;
		//for (int i = 0; i < SIZE_OF_ALPHABET; ++i)
		//	if(this->edges[i].to != ref.edges[i].to) return false;
		return false;
	}

	friend std::ostream& operator<< (std::ostream& out, const State& s) {
		out << s.vertex << '\t';	
		for (int i = 0; i < SIZE_OF_ALPHABET; ++i)
			if( s.edges[i].to != 0 )
				out << s.edges[i].to << '\t';
			else
				out << "-\t";
		return out;
	}
};

void print(string s, vector<State> v ) {
	cout << s << endl;
	for (int i = 0; i < v.size(); ++i)
		cout << '\t' << v[i].type() << v[i] << endl;
}

vector<State> acceptStates( vector<State> s) {
	vector<State> v;
	for (int i = 0; i < s.size(); ++i)
		if( s[i].Accept )
			v.push_back(s[i]);
	return v;
}
vector<State> normalStates( vector<State> s) {
	vector<State> v;
	for (int i = 0; i < s.size(); ++i)
		if( !s[i].Accept )
			v.push_back(s[i]);
	return v;
}

/*
	start with a list of vertex, edges so a table like 	
	STATE	a	b
	>1		2	4
	*2		4	7
	 3		5	-
	 4		8	-
	*5		4	3
	 6		5	4
	 7		2	-
	 8		2	4
	looks like the below, NOTE - is 0
*/
vector<State> exampleTable() {
	vector<State>  s;
	s.push_back(State());
	s.back().Start = true;
	s.back().Set(1,'a',2);
	s.back().Set(1,'b',4);
	s.push_back(State());
	s.back().Set(2,'a',4);
	s.back().Set(2,'b',7);
	s.back().Accept = true;
	s.push_back(State());
	s.back().Set(3,'a',5);
	s.back().Set(3,'b',0);
	s.push_back(State());
	s.back().Set(4,'a',8);
	s.back().Set(4,'b',0);
	s.push_back(State());
	s.back().Set(5,'a',4);
	s.back().Set(5,'b',3);
	s.back().Accept = true;
	s.push_back(State());
	s.back().Set(6,'a',5);
	s.back().Set(6,'b',4);
	s.push_back(State());
	s.back().Set(7,'a',2);
	s.back().Set(7,'b',0);
	s.push_back(State());
	s.back().Set(8,'a',2);
	s.back().Set(8,'b',4);
	return s;
}

vector<State> exampleTable2() {
	vector<State>  s;
	s.push_back(State());
	s.back().Start = true;
	s.back().Set(1,'a',2);
	s.back().Set(1,'b',3);

	s.push_back(State());
	s.back().Set(2,'a',0);
	s.back().Set(2,'b',4);

	s.push_back(State());
	s.back().Set(3,'a',4);
	s.back().Set(3,'b',3);
	s.back().Accept = true;

	s.push_back(State());
	s.back().Set(4,'a',4);
	s.back().Set(4,'b',4);
	s.back().Accept = true;

	return s;
}