//Linus Fackler
//LXF210001
//CS4384.501 - Automata Theory
//Dr. Willson
//
//This program reads a DFA from an input txt file and asks the user to enter
//a string, which will be tested, if it is accepted by the DFA.
//
//Using the transFunc, it traverses through the DFA until the string ends.
//If it ends in a final state, it is accepted. If not, the entered String
//is not in the language of the DFA.

#include <algorithm>
#include "DFA.hpp"
#include <fstream>

// added function "dfa_is_accepted"
bool dfa_is_accepted (const DFA &m, const std::string &w)
{
	int size = w.length();

	int state = m.initialState;		// we start with initial state
	int newState;					// temp new state
	int a;

	for (int i = 0; i < size; i++)
	{
		a = w[i] - '0';				// converting char to int
		newState = m.transFunc[a][state];	// temp new state we entered
		state = newState;			// new state is now current state
	}

	return m.finalStates[state];	// return wether string is accepted by DFA
}

std::ostream & operator << (std::ostream &out, const DFA &m) {
	out << m.numStates << std::endl;
	for (int q = 0; q < m.numStates; ++q) {
		out << m.transFunc [0][q] << ' ' << m.transFunc [1][q] << std::endl;
	}
	out << m.initialState << std::endl;
	out << std::count (m.finalStates.begin (), m.finalStates.end (), true) << ' ';
	for (int i = 0; i < m.numStates; ++i) {
		if (m.finalStates [i])
			out << i << ' ';
	}
	out << std::endl;
	return out;
}

std::istream & operator >> (std::istream &in, DFA &m) {
	in >> m.numStates;
	boost::multi_array<int, 2>::extent_gen extents;
	m.transFunc.resize (extents [2][m.numStates]);
	for (int q = 0; q < m.numStates; ++q) {
		in >> m.transFunc [0][q];
		in >> m.transFunc [1][q];
	}
	in >> m.initialState;
	m.finalStates.resize (m.numStates, false);
	int k;
	in >> k;
	for (int i = 0; i < k; ++i) {
		int q;
		in >> q;
		m.finalStates [q] = true;
	}
	return in;
}

int main()
{
	std::ifstream infile;
	infile.open("input.txt");	// opening input file located in same directory

	DFA dfa;					// create local DFA object
	infile >> dfa;				// reading in DFA from input file

	std::cout << dfa;			// printing out DFA to console

	std::cout << "What string do you want to test:\n";
	std::string st;
	std::cin >> st;				// letting user enter string to test


	bool isTrue = dfa_is_accepted(dfa, st);
		// checks wether string is accepted by DFA

	if (isTrue)
		std::cout << "Is accepted\n";

	else
		std::cout << "Not accepted\n";

	infile.close();
	return 0;
}
