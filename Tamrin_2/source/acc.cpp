#include "acc.h"

acc::acc(string user, string ip, unsigned int cardNum)
{
    setUser(user);
    setIP(ip);
    setCardNum(cardNum);
    setOpenDate();
    setExpDate();
}

void acc::setUser(string User)
{
    user = User;
}

string acc::getUser()
{
    return user;
}

void acc::setIP(string IP)
{
    ip.push_back(IP);
}

std::vector<std::string> acc::getIP()
{
    return ip;
}

void acc::printIP()
{
    for (int i = 0; i < ip.size(); i++)
        cout << ip[i] << endl;
}

void acc::setCardNum(unsigned int CardNum)
{
    cardNum = CardNum;
}

unsigned int acc::getCardNum()
{
    return cardNum;
}

void acc::changeBalance(long double money)
{
    balance = balance + money;
}

long double acc::getBalance()
{
    return balance;
}

void acc::setOpenDate()
{
    openDate = time(0);
}

time_t acc::getOpenDate()
{
    return openDate;
}

void acc::setExpDate()
{
    expDate = time(0) + (365 * 2 * 24 * 60 * 60);
}

time_t acc::getExpDate()
{
    return expDate;
}

void acc::addTransaction(transaction trans)
{
    transactions.push_back(trans);
}

vector<transaction> acc::getTransaction()
{
    return transactions;
}

void acc::changeDebt(long double amount)
{
    debt = debt + amount;
}

long double acc::getDebt()
{
    return debt;
}
