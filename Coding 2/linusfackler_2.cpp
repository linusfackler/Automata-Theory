// Linus Fackler
// LXF210001
// CS4384.501 - Automata Theory
// Dr. Wilson
//
// This program reads in multiple DFA's and runs them through different tests.
// It gets tested whether the DFA accepts a certain string w passed into the
// function, wheter the language of the DFA is empty, the set of all strings,
// and infinite. It also tests whether two certain languages are equal and
// whether one is a subset of the other.

#include "DFA_2.hpp"

bool reachesFinal(const DFA &m, int currentState, int depth);

// Does m accept string w?
// Same as in coding 1
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
		if (m.finalStates[i])
			nofinalstates = false;

	if (nofinalstates) return true;		// if the DFA has no final states,
										// L(m) is empty

	else if (reachesFinal(m, m.initialState, 0) || reachesFinal(m, m.initialState, 0))
		return false;
	// this function tests if any path in the DFA leads to a final state

	else
		return true;
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