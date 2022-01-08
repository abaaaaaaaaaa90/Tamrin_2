#include <vector>
#include <iostream>
#include <string>
#include "acc.h"
#include "funcs.h"

using namespace std;

int main()
{
    vector<acc> accs;
    int bankruptcy = 0;
    string strCmd;
    cout << "Enter your command:\n>>> ";
    getline(cin, strCmd);
    while (strCmd != "exit")
    {
        if (strCmd.size() != 0 && (int)strCmd[0] != 32)
        {
            char *chCmd = new char[strCmd.length() + 1]; 
            strcpy(chCmd, strCmd.c_str());               
            spltCmd(chCmd, accs, bankruptcy);
            delete[] chCmd;
        }
        else
            cout << "Did not get it" << endl;
        cout << "Enter your command:\n>>> ";
        getline(cin, strCmd);
    }

    return 0;
}