#include <stdexcept>
#include <cctype>
#include <cstring>
#include <random>

#include "acc.h"

void spltCmd(char *&, vector<acc> &, const int &);

unsigned int cardNum( vector<acc> &);

int isUser( string &);

int ipValid(const string &);

int searchUser( string &,  vector<acc>);

int searchIP(const string &, vector<acc> &);

void addTransaction(int, string, string, long double, int, vector<acc> &, int);

int print(string, vector<acc> &);

int fail( long double &,  vector<acc> &);

void getLoan( string &,  string &,  long double &, vector<acc> &);

void payLoan( string &,  long double &, vector<acc> &);

void addProfits( string &, vector<acc> &);

void transfer( string &,  string &, string &,  long double &, vector<acc> &);

void depositAndWithdraw(int,  string &,  string &,  long double &, vector<acc> &, int &);

void renewal( string &,  string &, vector<acc> &);

void addIP( string &,  string &, vector<acc> &);

void addAcc(string &, char *&, vector<acc> &);
