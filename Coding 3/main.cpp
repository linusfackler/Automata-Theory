#include "DFA_3.hpp"
#include <iostream>
#include <string>

using namespace std;

int main()
{
    DFA test;
    cout << "Enter DFA: " << endl;
    cin >> test;

    cout << endl << "Your entered DFA: " << endl << test << endl;

    string w = dfa_to_string(test);
    cout << endl << endl << "now string to DFA: " << endl;
    DFA test2 = dfa_from_string("00101010101010101");

    cout << endl << test2 << endl;

    cout << "DFA accepts itself: " << endl;
    cout << dfa_accepts_self(test) << endl;

    return 0;
}