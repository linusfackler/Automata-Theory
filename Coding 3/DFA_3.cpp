// Linus Fackler
// LXF210001
// CS4384.501 - Automata Theory
// Dr. Wilson
//
// Coding 3 assignment
//
// This program has 3 additional functions to our original Coding 2 assignment.
// These functions make it possible for a DFA to be converted into a single
// bit string that is completely unique to such DFA. It also allows us to 
// create a unique DFA out of such bit string.
// 
// Function "dfa_to_string" accepts a DFA and prints out its format into a 
// string stream. That way we get the exact format that we usually use to read
// in or print out a DFA. An example would be:
// 4
// 0 1
// 1 2
// 2 3
// 3 0
// 0
// 1 0
// This is all going to be stored in 1 single string. It then iterates through the
// entire string and converts each character, such as "4" or " "(space) to an 8 bit
// string in ascii representation. (where dec 32 is a space for example, so 32 in binary)
// It adds each 8 bit string to a final string and then returns this string
// This final bit string contains a multiple of 8 bits where each 8 bits represent a
// character in the DFA string, as seen above.
//
// Function "dfa_from_string" accepts a bit string and returns a DFA.
// First, it checks if the bit string entered is a valid bit string.
// It does this by checking if the number of bits in the string is a multiple of 8.
// Then, it checks if each character of the string is a 0 or a 1.
// If either is false, it exits with an error message.
// Then, using a stringstream made from the original string, it iterates through
// the stringstream and converts every 8 bits to a single character in its ascii
// representation. It adds every character (such as 4 or space) to the dfaString.
// Once this is done, it creates a DFA from the dfaString, that is in valid format.
// It then returns this DFA.
//
// The function "dfa_accepts_self" uses the "dfa_to_string" function to create a string
// from the passed in DFA and then returns the outcome of function "dfa_is_accepted", that
// determines whether the passed in string is accepted in the passed in DFA.


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

// -------------------------------------------------------------------
// -------------------------------------------------------------------
// Coding 3 functions:
// -------------------------------------------------------------------
// -------------------------------------------------------------------

// Given a DFA, convert it to a bit string
std::string dfa_to_string (const DFA &m)
{
	std::stringstream s;
	s << m;					// print dfa to stringstream, so we have dfa in single
							// string format the way it is defined in program
	std::string bitStr = "";	// creating empty string that will be final

	for (int i = 0; i < s.str().size(); i++)
	{
		bitStr += std::bitset<8>(s.str()[i]).to_string();
		// making 8 bit string out of each single character of dfa string
		// adding it to out final bit string
	}
	return bitStr;			// return final bit string
}

// Given a bit string, convert it to a DFA
DFA dfa_from_string (const std::string &w)
{
	std::stringstream sstream(w);
    std::string dfaString;
	int len = w.length();

	// -- CHECKING TO SEE IF BIT STRING NOT VALID --
	// checking if the string is not a multiple of 8, making the ascii
	// conversion impossible
	// if it is not, exit with error message
	if (len % 8 != 0)
	{
		std::cout << "Bitstring is not a valid bit-string to represent a DFA."
			<< std::endl << "Will exit now." << std::endl << std::endl;
		exit(EXIT_FAILURE);
	}
	// checking if any of the strings characters is not a 0 or 1
	// if it is, exit with error message
	for (int i = 0; i < len; i++)
	{
		if (w[i] != '0' && w[i] != '1')
		{
			std::cout << "Bitstring is not a valid bit-string to represent a DFA."
				<< std::endl << "Will exit now." << std::endl << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	// while it hasn't iterated through entire stringstream yet
	while(sstream.good())
	{
		std::bitset<8> bits;		
        sstream >> bits;
        char c = char(bits.to_ulong());		// making single character out of 8 bit string
        dfaString += c;				// adding each ascii character to dfa string
	}

	DFA m;
	std::stringstream (dfaString) >> m;		// creating a dfa from dfa string
	return m;								// return final dfa
}

// Given a DFA, does it accept its own encoding?
bool dfa_accepts_self (const DFA &m)
{
	// convert referenced DFA to string using our new method
	std::string s = dfa_to_string(m);

	// checks whether the DFA accepts its own bitstring representation
	return dfa_is_accepted(m, s);
}