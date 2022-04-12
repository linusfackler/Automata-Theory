#ifndef DFA_HPP
#define DFA_HPP

#include <iostream>
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>

/* numStates is the number of states in the DFA
 *
 * transFunc is the transition function.
 * For simplicity, we number the states 0..numStates-1, and assume the alphabet is {0,1}.
 * transFunc (1, 5) would be the new state we enter if we encounter a 1 in state 5.
 *
 * initialState is the initial state.
 *
 * finalStates is the set of final states.
 * finalStates [q] is true if q is a final state, false otherwise.
 */
struct DFA {
	int numStates;
	boost::numeric::ublas::matrix<int> transFunc;
	int initialState;
	std::vector<bool> finalStates;
};

// Returns a DFA whose language is the complement of L(m)
DFA dfa_complement (const DFA &m);

// Returns a DFA whose language is L(m1) union L(m2)
DFA dfa_union (const DFA &m1, const DFA &m2);

// Returns a DFA whose language is L(m1) intersection L(m2)
DFA dfa_intersection (const DFA &m1, const DFA &m2);

// Returns a DFA whose language is L(m1) - L(m2)
DFA dfa_difference (const DFA &m1, const DFA &m2);

/* Read or write a DFA in a specific format:
 * an integer, representing the number of states in the DFA
 * two integers for each state, the new states reached after reading a 0 or 1
 * an integer representing the initial state
 * the number of final states
 * a list of the final states
 *
 * For example, we could represent a DFA for strings in which the number of 1 bits is a multiple of 4:
 * 4
 * 0 1
 * 1 2
 * 2 3
 * 3 0
 * 0
 * 1 0
 */
std::ostream & operator << (std::ostream &out, const DFA &m); 
std::istream & operator >> (std::istream &in, DFA &m);

// Does m accept string w?
bool dfa_is_accepted (const DFA &m, const std::string &w);

// Does m fail to accept any strings? That is, is L(m) the empty set?
bool dfa_is_empty (const DFA &m);

// Does m accept every string? That is, is L(m) the set of all strings?
bool dfa_is_all (const DFA &m);

// Is L(m) infinite?
bool dfa_is_infinite (const DFA &m);

// Is L(m1) equal to L(m2)?
bool dfa_is_equal (const DFA &m1, const DFA &m2);

// Is L(m1) a subset of L(m2)?
bool dfa_is_subset (const DFA &m1, const DFA &m2);

#endif

