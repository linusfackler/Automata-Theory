#ifndef DFA_HPP
#define DFA_HPP

#include <iostream>
#include <string>
#include <boost/multi_array.hpp>

/* numStates is the number of states in the DFA
 *
 * transFunc is the transition function.
 * For simplicity, we number the states 0..numStates-1, and assume the alphabet is {0,1}.
 * transFunc [1][5] would be the new state we enter if we encounter a 1 in state 5.
 *
 * initialState is the initial state.
 *
 * finalStates is the set of final states.
 * finalStates [q] is true if q is a final state, false otherwise.
 */
struct DFA {
	int numStates;
	boost::multi_array<int, 2> transFunc;
	int initialState;
	std::vector<bool> finalStates;
};

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

// Determines if machine m accepts string w.
bool dfa_is_accepted (const DFA &m, const std::string &w);
#endif