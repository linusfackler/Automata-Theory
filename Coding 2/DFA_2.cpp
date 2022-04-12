
#include <algorithm>

#include "DFA_2.hpp"

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

