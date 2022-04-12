#include <iostream>
#include <sstream>

#include "DFA_2.hpp"

bool doTestCase (const std::string &s, bool myAnswer, bool correctAnswer) {
	const bool result = (myAnswer == correctAnswer);
	std::cout << s << (result ? "PASS" : "FAIL") << std::endl;
	return result;
}

int main () {
	DFA empty; // One state, accepts nothing
	std::stringstream ("1 0 0 0 0") >> empty;

	DFA all; // One state, accepts everything
	std::stringstream ("1 0 0 0 1 0") >> all;

	DFA empty10; // Ten states, accepts nothing
	std::stringstream ("10 1 2 3 4 5 6 7 8 9 0 2 4 6 8 0 1 3 5 7 9 0 0") >> empty10;

	DFA all10; // Ten states, accepts everything
	std::stringstream ("10 1 2 3 4 5 6 7 8 9 0 2 4 6 8 0 1 3 5 7 9 0 10 0 1 2 3 4 5 6 7 8 9") >> all10;

	DFA beg0; // Begins with 0
	std::stringstream ("3 1 2 1 1 2 2 0 1 1") >> beg0;

	DFA beg1; // Begins with 1
	std::stringstream ("3 2 1 1 1 2 2 0 1 1") >> beg1;

	DFA end0; // Ends with 0
	std::stringstream ("3 1 2 1 2 1 2 0 1 1") >> end0;

	DFA end1; // Ends with 1
	std::stringstream ("3 1 2 1 2 1 2 0 1 2") >> end1;

	DFA begEndSame; // Begin and end with same symbol
	std::stringstream ("5 1 3 1 2 1 2 4 3 4 3 0 2 1 3") >> begEndSame;

	DFA begEndSameComplex; // Begin and end with same symbol, complicated version
	begEndSameComplex = dfa_union (dfa_intersection (beg0, end0), dfa_intersection (beg1, end1));

	DFA beg01; // Begins with both 0 and 1 (impossible)
	beg01 = dfa_intersection (beg0, beg1);

	DFA end01; // Ends with both 0 and 1 (impossible)
	end01 = dfa_intersection (end0, end1);

	DFA len5; // Length is exactly 5
	std::stringstream ("7 1 1 2 2 3 3 4 4 5 5 6 6 6 6 0 1 5") >> len5;

	DFA len5garbage; // Length is exactly 5, with some garbage states at the end
	std::stringstream ("11 1 1 2 2 3 3 4 4 5 5 6 6 6 7 8 9 10 6 7 8 9 10 0 1 5") >> len5garbage;

	DFA num0is5; // Exactly 5 0s
	std::stringstream ("7 1 0 2 1 3 2 4 3 5 4 6 5 6 6 0 1 5") >> num0is5;

	DFA lenCon5mod6; // Length is congruent to 5, mod 6
	std::stringstream ("6 1 1 2 2 3 3 4 4 5 5 0 0 0 1 5") >> lenCon5mod6;

	DFA num0Con5mod6; // Number of 0s is congruent to 5, mod 6
	std::stringstream ("6 1 0 2 1 3 2 4 3 5 4 0 5 0 1 5") >> num0Con5mod6;

	DFA even0and1; // An even number of 0s, followed by a 1
	std::stringstream ("4 3 3 2 0 1 3 3 3 1 1 0") >> even0and1;

	DFA oddNum0s; // An odd number of 0s
	std::stringstream ("2 1 0 0 1 1 1 0") >> oddNum0s;

	DFA evenNum0s; // An even number of 0s
	std::stringstream ("2 1 0 0 1 1 1 1") >> evenNum0s;

	DFA evenOrOdd0s; // An even or an odd number of 0s (all bit strings)
	evenOrOdd0s = dfa_union (oddNum0s, evenNum0s);

	DFA thingA; // (00*1)*1(0U1)*
	std::stringstream ("3 1 2 1 0 2 2 0 1 2") >> thingA;

	DFA thingB; // (01*0)*1(0U1)*
	std::stringstream ("3 1 2 0 1 2 2 0 1 2") >> thingB;

	DFA trollMachine; // Lots of states, but never leaves the initial state
	std::stringstream ("10 1 2 4 5 8 9 3 3 0 7 6 1 5 7 9 2 4 6 8 0 3 1 3") >> trollMachine;

	char c;

	bool isAcceptedResult = true;
	std::cout << "\nTest is_accepted?" << std::endl;
	std::cin >> c;
	if (c == 'y') {
		std::cout << "Testing is_accepted" << std::endl;
		isAcceptedResult = doTestCase ("0110 begins with 0: ", dfa_is_accepted (beg0, "0110"), true) && isAcceptedResult;
		isAcceptedResult = doTestCase ("0110 begins with 1: ", dfa_is_accepted (beg1, "0110"), false) && isAcceptedResult;
		isAcceptedResult = doTestCase ("0110 begins and ends same: ", dfa_is_accepted (begEndSame, "0110"), true) && isAcceptedResult;
		isAcceptedResult = doTestCase ("100100100 begins and ends same: ", dfa_is_accepted (begEndSame, "100100100"), false) && isAcceptedResult;
		isAcceptedResult = doTestCase ("10101010101010101 length is congruent to 5 (mod 6): ", dfa_is_accepted (lenCon5mod6, "10101010101010101"), true) && isAcceptedResult;
		isAcceptedResult = doTestCase ("10101010101010101 num 0s is congruent to 5 (mod 6): ", dfa_is_accepted (num0Con5mod6, "10101010101010101"), false) && isAcceptedResult;
		isAcceptedResult = doTestCase ("0000000000001 is in (00)*1: ", dfa_is_accepted (even0and1, "0000000000001"), true) && isAcceptedResult;
		isAcceptedResult = doTestCase ("length of 0000000000001 is 5: ", dfa_is_accepted (len5garbage, "0000000000001"), false) && isAcceptedResult;
		isAcceptedResult = doTestCase ("11001001, universal acceptance: ", dfa_is_accepted (all, "11001001"), true) && isAcceptedResult;
		isAcceptedResult = doTestCase ("11001001, universal rejection: ", dfa_is_accepted (empty, "11001001"), false) && isAcceptedResult;
		std::cout << "Overall result: " << (isAcceptedResult ? "PASS" : "FAIL") << '\n' << std::endl;
	} else {
		std::cout << "Not testing is_accepted\n" << std::endl;
		isAcceptedResult = false;
	}


	bool isEmptyResult = true;
	std::cout << "Test is_empty?" << std::endl;
	std::cin >> c;
	if (c == 'y') {
		std::cout << "Testing is_empty" << std::endl;
		isEmptyResult = doTestCase ("1 state, accepts nothing: ", dfa_is_empty (empty), true) && isEmptyResult;
		isEmptyResult = doTestCase ("1 state, accepts everything: ", dfa_is_empty (all), false) && isEmptyResult;
		isEmptyResult = doTestCase ("10 states, accepts nothing: ", dfa_is_empty (empty10), true) && isEmptyResult;
		isEmptyResult = doTestCase ("10 states, accepts everything: ", dfa_is_empty (all10), false) && isEmptyResult;
		isEmptyResult = doTestCase ("Begins both 0 and 1: ", dfa_is_empty (beg01), true) && isEmptyResult;
		isEmptyResult = doTestCase ("Ends both 0 and 1: ", dfa_is_empty (end01), true) && isEmptyResult;
		isEmptyResult = doTestCase ("Begins and ends same symbol: ", dfa_is_empty (begEndSame), false) && isEmptyResult;
		isEmptyResult = doTestCase ("(00)*1: ", dfa_is_empty (even0and1), false) && isEmptyResult;
		isEmptyResult = doTestCase ("Length is 5: ", dfa_is_empty (len5), false) && isEmptyResult;
		isEmptyResult = doTestCase ("Number of 0s is 5: ", dfa_is_empty (num0is5), false) && isEmptyResult;
		std::cout << "Overall result: " << (isEmptyResult ? "PASS" : "FAIL") << '\n' << std::endl;
	} else {
		std::cout << "Not testing is_empty\n" << std::endl;
		isEmptyResult = false;
	}

	bool isAllResult = true;
	std::cout << "Test is_all?" << std::endl;
	std::cin >> c;
	if (c == 'y') {
		std::cout << "Testing is_all" << std::endl;
		isAllResult = doTestCase ("1 state, accepts nothing: ", dfa_is_all (empty), false) && isAllResult;
		isAllResult = doTestCase ("1 state, accepts everything: ", dfa_is_all (all), true) && isAllResult;
		isAllResult = doTestCase ("10 states, accepts nothing: ", dfa_is_all (empty10), false) && isAllResult;
		isAllResult = doTestCase ("10 states, accepts everything: ", dfa_is_all (all10), true) && isAllResult;
		isAllResult = doTestCase ("Begins and ends same symbol: ", dfa_is_all (begEndSame), false) && isAllResult;
		isAllResult = doTestCase ("Even or Odd number of 0s: ", dfa_is_all (evenOrOdd0s), true) && isAllResult;
		isAllResult = doTestCase ("(00)*1: ", dfa_is_all (even0and1), false) && isAllResult;
		isAllResult = doTestCase ("Length is 5: ", dfa_is_all (len5), false) && isAllResult;
		isAllResult = doTestCase ("Number of 0s is 5: ", dfa_is_all (num0is5), false) && isAllResult;
		isAllResult = doTestCase ("Troll Machine: ", dfa_is_all (trollMachine), true) && isAllResult;
		std::cout << "Overall result: " << (isAllResult ? "PASS" : "FAIL") << '\n' << std::endl;
	} else {
		std::cout << "Not testing is_all\n" << std::endl;
		isAllResult = false;
	}

	bool isInfiniteResult = true;
	std::cout << "Test is_infinite?" << std::endl;
	std::cin >> c;
	if (c == 'y') {
		std::cout << "Testing is_infinite" << std::endl;
		isInfiniteResult = doTestCase ("1 state, accepts nothing: ", dfa_is_infinite (empty), false) && isInfiniteResult;
		isInfiniteResult = doTestCase ("1 state, accepts everything: ", dfa_is_infinite (all), true) && isInfiniteResult;
		isInfiniteResult = doTestCase ("10 states, accepts nothing: ", dfa_is_infinite (empty10), false) && isInfiniteResult;
		isInfiniteResult = doTestCase ("10 states, accepts everything: ", dfa_is_infinite (all10), true) && isInfiniteResult;
		isInfiniteResult = doTestCase ("Begins and ends same symbol: ", dfa_is_infinite (begEndSame), true) && isInfiniteResult;
		isInfiniteResult = doTestCase ("Length is 5: ", dfa_is_infinite (len5), false) && isInfiniteResult;
		isInfiniteResult = doTestCase ("Length is 5, extra states: ", dfa_is_infinite (len5garbage), false) && isInfiniteResult;
		isInfiniteResult = doTestCase ("Number of 0s is 5: ", dfa_is_infinite (num0is5), true) && isInfiniteResult;
		isInfiniteResult = doTestCase ("(00)*1: ", dfa_is_infinite (even0and1), true) && isInfiniteResult;
		isInfiniteResult = doTestCase ("Begins both 0 and 1: ", dfa_is_infinite (beg01), false) && isInfiniteResult;
		std::cout << "Overall result: " << (isInfiniteResult ? "PASS" : "FAIL") << '\n' << std::endl;
	} else {
		std::cout << "Not testing is_infinite\n" << std::endl;
		isInfiniteResult = false;
	}

	bool isEqualResult = true;
	std::cout << "Test is_equal?" << std::endl;
	std::cin >> c;
	if (c == 'y') {
		std::cout << "Testing is_equal" << std::endl;
		isEqualResult = doTestCase ("1 state vs 10 state; accept nothing: ", dfa_is_equal (empty, empty10), true) && isEqualResult;
		isEqualResult = doTestCase ("1 state vs 10 state; accept everything: ", dfa_is_equal (all, all10), true) && isEqualResult;
		isEqualResult = doTestCase ("length 5 vs length 5 with garbage: ", dfa_is_equal (len5, len5garbage), true) && isEqualResult;
		isEqualResult = doTestCase ("Begin both 0 and 1 vs End both 0 and 1: ", dfa_is_equal (beg01, end01), true) && isEqualResult;
		isEqualResult = doTestCase ("Length 5 vs Length Congruent to 5 (mod 6): ", dfa_is_equal (len5, lenCon5mod6), false) && isEqualResult;
		isEqualResult = doTestCase ("Odd num 0s vs Exactly 5 0s: ", dfa_is_equal (oddNum0s, num0is5), false) && isEqualResult;
		isEqualResult = doTestCase ("Ends 1 vs Begins 0: ", dfa_is_equal (end1, beg0), false) && isEqualResult;
		isEqualResult = doTestCase ("Num 0s congruent to 5 (mod 6) vs Even num 0s: ", dfa_is_equal (num0Con5mod6, evenNum0s), false) && isEqualResult;
		isEqualResult = doTestCase ("Ends both 0 and 1 vs Begins both 0 and 1: ", dfa_is_equal (end01, beg01), true) && isEqualResult;
		isEqualResult = doTestCase ("(00*1)*1(0U1)* vs (01*0)*1(0U1)*: ", dfa_is_equal (thingA, thingB), false) && isEqualResult;
		isEqualResult = doTestCase ("Begin and end same; simple vs complex: ", dfa_is_equal (begEndSameComplex, begEndSame), true) && isEqualResult;
		std::cout << "Overall result: " << (isEqualResult ? "PASS" : "FAIL") << '\n' << std::endl;
	} else {
		std::cout << "Not testing is_equal\n" << std::endl;
		isEqualResult = false;
	}

	bool isSubsetResult = true;
	std::cout << "Test is_subset?" << std::endl;
	std::cin >> c;
	if (c == 'y') {
		std::cout << "Testing is_subset" << std::endl;
		isSubsetResult = doTestCase ("Length 5 vs Length Congruent to 5 (mod 6): ", dfa_is_subset (len5, lenCon5mod6), true) && isSubsetResult;
		isSubsetResult = doTestCase ("(00)*1 vs Ends 1: ", dfa_is_subset (even0and1, end1), true) && isSubsetResult;
		isSubsetResult = doTestCase ("Exactly 5 0s vs Num 0s congruent to 5 (mod 6): ", dfa_is_subset (num0is5, num0Con5mod6), true) && isSubsetResult;
		isSubsetResult = doTestCase ("Num 0s congruent to 5 (mod 6) vs Odd Num 0s: ", dfa_is_subset (num0Con5mod6, oddNum0s), true) && isSubsetResult;
		isSubsetResult = doTestCase ("Ends 1 vs Begins 0: ", dfa_is_subset (end1, beg0), false) && isSubsetResult;
		isSubsetResult = doTestCase ("Odd num 0s vs Exactly 5 0s: ", dfa_is_subset (oddNum0s, num0is5), false) && isSubsetResult;
		isSubsetResult = doTestCase ("Even num 0s vs Begins 1: ", dfa_is_subset (evenNum0s, beg1), false) && isSubsetResult;
		isSubsetResult = doTestCase ("Empty vs All: ", dfa_is_subset (empty, all), true) && isSubsetResult;
		isSubsetResult = doTestCase ("All vs Empty: ", dfa_is_subset (all, empty), false) && isSubsetResult;
		isSubsetResult = doTestCase ("(00*1)*1(0U1)* vs (01*0)*1(0U1)*: ", dfa_is_subset (thingA, thingB), false) && isSubsetResult;
		isSubsetResult = doTestCase ("Begin and end same; simple vs complex: ", dfa_is_subset (begEndSame, begEndSameComplex), true) && isSubsetResult;
		std::cout << "Overall result: " << (isSubsetResult ? "PASS" : "FAIL") << '\n' << std::endl;
	} else {
		std::cout << "Not testing is_subset\n" << std::endl;
		isEqualResult = false;
	}

	std::cout << "Summary: \n"
		<< "is_accepted: " << (isAcceptedResult ? "PASS" : "FAIL") << '\n'
		<< "is_empty: " << (isEmptyResult ? "PASS" : "FAIL") << '\n'
		<< "is_all: " << (isAllResult ? "PASS" : "FAIL") << '\n'
		<< "is_infinite: " << (isInfiniteResult ? "PASS" : "FAIL") << '\n'
		<< "is_equal: " << (isEqualResult ? "PASS" : "FAIL") << '\n'
		<< "is_subset: " << (isSubsetResult ? "PASS" : "FAIL") << '\n'
		<< std::endl;
	
	return 0;
}

