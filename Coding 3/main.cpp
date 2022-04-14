#include "DFA_3.hpp"
#include <iostream>
#include <string>

using namespace std;

int main()
{
    char choice = 'y';
    while(choice == 'y')
    {
        cout << "enter string to check if DFA is valid: " << endl;
        string k;
        cin >> k;
        DFA test3 = dfa_from_string(k);

        cout << test3;

        cout << "wanna try again?" << endl;
        cin >> choice;
    }
    return 0;
}