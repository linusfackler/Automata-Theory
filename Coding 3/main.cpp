#include "DFA_3.hpp"
#include <iostream>
#include <string>

using namespace std;

int main()
{
    char choice = 'y';
    while(choice == 'y')
    {

        DFA m;
	    
        cout << "enter DFA: " << endl;
        string k;
        cin >> k;
        stringstream (k) >> m;

        cout << m << endl;

        string p = dfa_to_string(m);
        cout << endl << p << endl;

        DFA yourmom = dfa_from_string(p);

        cout << endl << "DFA from string: " << endl;
        cout << yourmom;

        cout << endl << endl << "does it accept itself?" << endl;
        cout << dfa_accepts_self(yourmom);
        cout << endl << endl;

        cout << "wanna try again?" << endl;
        cin >> choice;
    }
    return 0;
}