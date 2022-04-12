// comments


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
    cout << "Dfa to string so far: " << endl << endl;

    string w = dfa_to_string(test);
    cout << endl << endl << "now string to DFA: " << endl;
    DFA test2 = dfa_from_string(w);

    cout << endl << test2 << endl;

    return 0;
}