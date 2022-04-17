#include "DFA_3.hpp"
#include <iostream>
#include <string>

using namespace std;

int main()
{
    DFA m;
    
    cout << "enter DFA: " << endl;

    cin >> m;

    cout << m << endl;

    string p = dfa_to_string(m);
    cout << endl << p << endl;

    DFA yourmom = dfa_from_string(p);

    cout << endl << "DFA from string: " << endl;
    cout << yourmom;

    cout << endl << endl << "does it accept itself?" << endl;
    cout << dfa_accepts_self(yourmom);
    cout << endl << endl;

    // cout << "wanna try again?" << endl;
    // cin >> choice;
    return 0;
}