#include <algorithm>
#include <string>
#include <iostream> 
#include <sstream>
#include <bitset>

#include "DFA_3.hpp"

DFA dfa_complement (const DFA &m)
{
	DFA m2 = m;
	for (auto it = m2.finalStates.begin (); it != m2.finalStates.end (); ++it)
		*it = !*it;
	return m2;
}

DFA dfa_crossproduct_helper (const DFA &m1, const DFA &m2)
{
	DFA m3;
	m3.numStates = m1.numStates * m2.numStates;
	m3.transFunc.resize (2, m3.numStates);
	m3.initialState = m1.initialState + m1.numStates * m2.initialState;
	for (int i = 0; i < m1.numStates; ++i)
		for (int j = 0; j < m2.numStates; ++j)
			for (int x : {0, 1})
				m3.transFunc (x, i + m1.numStates * j) = m1.transFunc (x, i) + m1.numStates * m2.transFunc (x, j);
	m3.finalStates.resize (m3.numStates);
	return m3;
}

DFA dfa_union (const DFA &m1, const DFA &m2)
{
	DFA m3 = dfa_crossproduct_helper (m1, m2);
	for (int i = 0; i < m1.numStates; ++i)
		for (int j = 0; j < m2.numStates; ++j)
			m3.finalStates [i + m1.numStates * j] = m1.finalStates [i] || m2.finalStates [j];
	return m3;
}

DFA dfa_intersection (const DFA &m1, const DFA &m2)
{
	DFA m3 = dfa_crossproduct_helper (m1, m2);
	for (int i = 0; i < m1.numStates; ++i)
		for (int j = 0; j < m2.numStates; ++j)
			m3.finalStates [i + m1.numStates * j] = m1.finalStates [i] && m2.finalStates [j];
	return m3;
}

DFA dfa_difference (const DFA &m1, const DFA &m2)
{
	return dfa_intersection (m1, dfa_complement (m2));
}

std::ostream & operator << (std::ostream &out, const DFA &m) {
	out << m.numStates << std::endl;
	for (int q = 0; q < m.numStates; ++q) {
		out << m.transFunc (0, q) << ' ' << m.transFunc (1, q) << std::endl;
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
	m.transFunc.resize (2, m.numStates);
	for (int q = 0; q < m.numStates; ++q) {
		in >> m.transFunc (0, q);
		in >> m.transFunc (1, q);
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

// -------------------------------
// Coding 2 functions:
// -------------------------------

bool reachesFinal(const DFA &m, int currentState, int depth);

// Does m accept string w?
bool dfa_is_accepted (const DFA &m, const std::string &w)
{
	int size = w.length();

	int state = m.initialState;		// we start with initial state
	int newState;					// temp new state
	int a;

	for (int i = 0; i < size; i++)
	{
		a = w[i] - '0';				// converting char to int
		newState = m.transFunc(a, state);	// temp new state we entered
		state = newState;			// new state is now current state
	}

	return m.finalStates[state];	// return wether string is accepted by DFA
}

// Does m accept any strings? That is, is L(m) the empty set?
bool dfa_is_empty (const DFA &m)
{
	int size = m.numStates;
	bool nofinalstates = true;

	for (int i = 0; i < size; i++)
	{
		if (m.finalStates[i])
		{
			nofinalstates = false;
		}
	}

	if (nofinalstates) return true;		// if the DFA has no final states,
										// L(m) is empty

	else if (reachesFinal(m, m.initialState, 0) || reachesFinal(m, m.initialState, 0))
	{
		return false;
	}
	// this function tests if any path in the DFA leads to a final state

	else
	{
		return true;
	}
	// if no final state is reached, L(m) is empty
}

// does the DFA reach a final state from the current state?
// depth covers the case where the DFA loops forever
bool reachesFinal(const DFA &m, int currentState, int depth)
{
	int next0 = m.transFunc(0, currentState);	// transition with next 0
	int next1 = m.transFunc(1, currentState);	// transition with next 1

	if (m.finalStates[next0] || m.finalStates[next1])
	{
		return true;
	}
	// base case: if any of the next states is final, language is not empty

	else
	{
		if (depth > 10)
		{
			return false;
		}
		// after 10 traverses to nodes that are not final states, you
		// can assume it is looping forever and not reaching a final state

		else if (reachesFinal(m, next0, ++depth) || reachesFinal(m, next1, ++depth))
		{
			return true;
		}
		// recursive call
	}
}


// Does m accept every string? That is, is L(m) the set of all strings?
bool dfa_is_all (const DFA &m)
{
	DFA m2 = dfa_complement(m);
	
	return dfa_is_empty(m2);
	// if the complement of m is empty, L(m) is the set of all strings
}

// Is L(m) infinite?
bool dfa_is_infinite (const DFA &m)
{
	if (dfa_is_all(m)) return true;
	// if L(m) is the set of all strings, L(m) is also empty

	else if (dfa_is_empty(m)) return false;
	// if L(m) is empty, it cannot be infinite

	else
	{
		// checks if the DFA loops infinitely at some point
		for (int i = 0; i < m.numStates - 1; i++)
		{
			for (int j = i; j < m.numStates; j++)
			{
				if (i == m.transFunc(0, j) || i == m.transFunc(1, j))
				{
					if (reachesFinal(m, i, 0))
						return true;
					// just because it loops, doesn't mean it's infinite
					// needs to be able to reach a final state
				}
			}

		}
	}
	return false;
}

// Is L(m1) equal to L(m2)?
bool dfa_is_equal (const DFA &m1, const DFA &m2)
{
	return (dfa_is_empty(dfa_difference(m1, m2)) && dfa_is_empty(dfa_difference(m2, m1)));
	// if the difference of m1 and m2 is empty, L(m1) is equal to L(m2)
	// needs to be checked both ways
}

// Is L(m1) a subset of L(m2)?
bool dfa_is_subset (const DFA &m1, const DFA &m2)
{
	DFA temp = dfa_intersection(m1, m2);
	return (dfa_is_equal(temp, m1));
	// if m1 is equal to the intersection of m1 and m2,
	// L(m1) is a subset of L(m2)
}


// -------------------------------
// Coding 3 functions:
// -------------------------------


// Given a DFA, convert it to a bit string
std::string dfa_to_string (const DFA &m)
{
	std::stringstream s;
	s << m;
	std::string bitStr = "";

	for (int i = 0; i < s.str().size(); i++)
	{
		bitStr += std::bitset<8>(s.str()[i]).to_string();
	}
	return bitStr;
}

// Given a bit string, convert it to a DFA
DFA dfa_from_string (const std::string &w)
{
	std::stringstream sstream(w);
    std::string dfaString;

	while(sstream.good())
	{
		std::bitset<8> bits;
        sstream >> bits;
        char c = char(bits.to_ulong());
        dfaString += c;
	}

	DFA m;
	std::stringstream (dfaString) >> m;
	return m;
}

// Given a DFA, does it accept its own encoding?
bool dfa_accepts_self (const DFA &m)
{

}