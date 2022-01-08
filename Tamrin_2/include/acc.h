#ifndef ACC_H
#define ACC_H

#include <iostream>
#include <ctime>
#include <string>
#include <vector>

using namespace std;

struct transaction
{
    bool deposit = false;
    bool withdraw = false;
    bool profit = false;
    string original;
    string goal;
    long double amount;
    time_t date;
    long double remain;
};

class acc
{
public:
    acc(string, string, unsigned int); 
    ~acc() {}                                    
    string getUser();
    void setIP(string);
    vector<string> getIP();
    void printIP();
    unsigned int getCardNum();
    void changeBalance(long double);
    long double getBalance();
    time_t getOpenDate();
    void setExpDate();
    time_t getExpDate();
    void addTransaction(transaction);
    vector<transaction> getTransaction();
    void changeDebt(long double);
    long double getDebt();
private:
    string user;
    void setUser(string);
    vector<string> ip;
    unsigned int cardNum;
    void setCardNum(unsigned int);
    long double balance = 0;
    time_t openDate;
    void setOpenDate();
    time_t expDate;
    vector<transaction> transactions;
    long double debt = 0;
};

#endif 
