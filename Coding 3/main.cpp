// comments


#include "DFA_3.hpp"
#include <iostream>

using namespace std;

int main()
{
    DFA test;
    cout << "Enter DFA: " << endl;
    cin >> test;

    cout << endl << "Your entered DFA: " << endl << test << endl;
    cout << "Dfa to string so far: " << endl << endl;

    cout << dfa_to_string(test);

    return 0;
}