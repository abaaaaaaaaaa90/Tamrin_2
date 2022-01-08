#include "acc.h"
#include "funcs.h"

using namespace std;

unsigned int cardNum(vector<acc> &accs)
{
    default_random_engine engine(static_cast<unsigned int>(time(0)));
    uniform_int_distribution<unsigned int> randNum(1000, 9999);
    int search = 0;
    unsigned int cardNum;
    while (search == 0)
    {
        search = 1;
        cardNum = randNum(engine);
        for (int i = 0; i < accs.size(); i++)
            if (accs[i].getCardNum() == cardNum)
            {
                search = 0;
                break;
            }
    }
    return cardNum;
}

int userValid(string &user)
{
    if (isdigit(user[0]))
        return 0;
    for (int i = 0; i < user.size(); i++)
        if (ispunct(user[i]))
            return 0;
    return 1;
}

int ipValid(const string &strIP)
{
    char *ip = new char[strIP.size() + 1];
    strcpy(ip, strIP.c_str());
    char *token = strtok(ip, ".");
    if (token == NULL)
    {
        delete[] ip;
        return 0;
    }
    int counter = 0;
    while (token != NULL)
    {
        counter++;
        string temp = token;
        for (int i = 0; i < temp.size(); i++)
            if (!isdigit(temp[i]))
            {
                delete[] ip;
                return 0;
            }
        int tempNum = stoi(temp);
        if (tempNum >= 254 || tempNum <= -1)
        {
            delete[] ip;
            return 0;
        }
        token = strtok(NULL, ".");
    }
    if (counter != 4)
    {
        delete[] ip;
        return 0;
    }
    delete[] ip;
    return 1;
}

int searchUser(string &user, vector<acc> accs)
{
    for (int i = 0; i < accs.size(); i++)
        if (accs[i].getUser() == user)
            return i;
    return -1;
}

int searchIP(const string &ip, vector<acc> &accs)
{
    vector<string> temp;
    for (int i = 0; i < accs.size(); i++)
    {
        temp = accs[i].getIP();
        for (int j = 0; j < temp.size(); j++)
            if (temp[j] == ip)
                return i;
    }
    return -1;
}

void addTransaction(int deposit, string original, string goal, long double amount, int index, vector<acc> &accs, int profit = 0)
{
    transaction temp;
    if (deposit == 1)
        temp.deposit = 1;
    else
        temp.withdraw = 1;
    if (profit)
        temp.profit = 1;
    temp.original = original;
    temp.goal = goal;
    temp.amount = amount;
    temp.date = time(0);
    temp.remain = accs[index].getBalance();
    accs[index].addTransaction(temp);
}

int print(string token, vector<acc> &accs)
{
    int index = searchUser(token, accs), search = 0;
    for (int i = 0; i < accs.size() && !search; i++)
    {
        if (to_string(accs[i].getCardNum()) == token)
        {
            index = i;
            search = 1;
        }
        vector<string> temp = accs[i].getIP();
        for (int j = 0; j < temp.size() && !search; j++)
            if (temp[j] == token)
            {
                index = i;
                search = 1;
            }
    }
    if (index != -1)
    {
        cout << "User : " << accs[index].getUser() << '\n'
             << "IP : \n";
        accs[index].printIP();
        cout << "Card num : " << accs[index].getCardNum() << '\n'
             << "Balance : " << accs[index].getBalance() << "$\n";
        time_t tempOpen = accs[index].getOpenDate();
        tm *tempTime = localtime(&tempOpen);
        cout << "Open date : " << (tempTime->tm_year + 1900) << '-' << (tempTime->tm_mon + 1) << '-' << (tempTime->tm_mday) << '\n';
        tempOpen = accs[index].getExpDate();
        tempTime = localtime(&tempOpen);
        cout << "Exp time : " << (tempTime->tm_year + 1900) << '-' << (tempTime->tm_mon + 1) << '-' << (tempTime->tm_mday) << '\n'
             << "Debt : " << accs[index].getDebt() << '\n';
        return 1;
    }
    return 0;
}

int checkBankruptcy(long double &amount, vector<acc> &accs)
{
    long double loans = 0, allBalance = 0, availableBalance = 0;
    for (int i = 0; i < accs.size(); i++)
    {
        loans = loans + accs[i].getDebt();
        allBalance = allBalance + accs[i].getBalance();
    }
    availableBalance = allBalance - loans;
    if (availableBalance < amount)
        return 1;
    return 0;
}

void payLoan(string &user, long double &amount, vector<acc> &accs)
{

    int index = searchUser(user, accs);
    if (index == -1)
        throw invalid_argument("Error : user not found");
    else if (amount < 0)
        throw invalid_argument("Error : invalid amount of money");
    if (accs[index].getDebt() == 0)
    {
        accs[index].changeBalance(amount);
        cout << "you have no debts and money added to your acc. your balance is : " << accs[index].getBalance() << "$\n";
    }
    else if (accs[index].getDebt() < amount)
    {
        long double debt = accs[index].getDebt();
        long double extra = amount - accs[index].getDebt();
        accs[index].changeDebt((-1) * debt);
        accs[index].changeBalance(extra);
        cout << "debt successfully paid. current debt is : " << accs[index].getDebt() << '\n'
             << extra << "$ was extra and added to your acc. your balance is : " << accs[index].getBalance() << "$\n";
    }
    else
    {
        accs[index].changeDebt((-1) * amount);
        cout << "debt successfully paid. current debt is : " << accs[index].getDebt() << '\n';
    }
}

void getLoan(string &user, string &ip, long double &amount, vector<acc> &accs)
{
    int index = searchUser(user, accs);
    if (index == -1)
        throw invalid_argument("Error: user not found");
    int ipIndex = searchIP(ip, accs);
    if (ipIndex == -1 || ipIndex != index)
        throw invalid_argument("Error: ip not found");
    if (time(0) >= accs[index].getExpDate())
        throw invalid_argument("Error: your acc has expired");
    if (amount < 0)
        throw invalid_argument("invalid amount");
    if (accs[index].getBalance() < (amount * 0.75))
        throw invalid_argument("you can't get the loan. you must have at least 0.75 of the money you requested");
    if (accs[index].getDebt() > 0)
        throw invalid_argument("you have unpaid loan");
    long double loans = 0, allbalance = 0;
    for (int i = 0; i < accs.size(); i++)
    {
        loans += accs[i].getDebt();
        allbalance += accs[i].getBalance();
    }
    long double availableBalance = allbalance - loans;
    if (availableBalance < amount)
        throw invalid_argument("Bank can't give a loan in this amount");
    accs[index].changeDebt(amount + (amount * 0.25));
    cout << "The loan successfully gave\n";
}

void addProfit(string &user, vector<acc> &accs)
{
    int index = searchUser(user, accs);
    if (index == -1)
        throw invalid_argument("Error: User not found");
    if (time(0) >= accs[index].getExpDate())
        throw invalid_argument("Error: your acc has expired");
    if (accs[index].getOpenDate() > (time(0) - 2592000))
        throw invalid_argument("you can't get the profit. acc must created for more than 30 days");
    vector<transaction> temp = accs[index].getTransaction();
    for (int i = 0; i < temp.size(); i++)
        if ((temp[i].date > (time(0) - 2592000)) && temp[i].profit)
            throw invalid_argument("you can't get the profit. you have got the profit in last 30 days");
    time_t now = time(0);
    int counter = 1;
    int hasWithdraw = 0;
    long double balance = accs[index].getBalance();
    for (int i = 0; i < temp.size(); i++)
        if ((temp[i].date > now - 604800) && (temp[i].date < now))
        {
            if (temp[i].withdraw)
                hasWithdraw = 1;
            counter++;
            balance += temp[i].remain;
        }
    long double profit;
    if (hasWithdraw != 0)
        profit = ((balance / counter) * 0.18) / 12;
    else
        profit = ((balance / counter) * 0.20) / 12;
    accs[index].changeBalance(profit);
    cout << profit << "$ added\n";
    addTransaction(1, "Bank", user, profit, index, accs, 1);
}

void transfer(string &senderUser, string &senderIP, string &receiver, long double &amount, vector<acc> &accs)
{
    int senderIndex = searchUser(senderUser, accs);
    if (senderIndex == -1)
        throw invalid_argument("Error: Sender user not found");
    int senderIpIndex = searchIP(senderIP, accs);
    if (senderIpIndex == -1 || senderIpIndex != senderIndex)
        throw invalid_argument("Error: Sender IP not found");
    int receiverIndex = searchUser(receiver, accs), receiverIpIndex = searchIP(receiver, accs);
    if (receiverIndex == -1 && receiverIpIndex == -1)
        throw invalid_argument("Error: Receiver user or ip not found");
    else if (amount <= 0)
        throw invalid_argument("Error: invalid amount of money");
    else if (time(0) >= accs[senderIndex].getExpDate())
    {
        cout << "Your acc has expired\n";
        renewal(senderUser, senderIP, accs);
        return;
    }
    if (accs[senderIndex].getBalance() < amount)
        throw invalid_argument("Error: your money is not enough");
    accs[senderIndex].changeBalance((-1) * amount);
    if (receiverIndex != -1)
    {
        accs[receiverIndex].changeBalance(amount);
        addTransaction(1, senderUser, receiver, amount, receiverIndex, accs);
    }
    else
    {
        accs[receiverIpIndex].changeBalance(amount);
        receiver = accs[receiverIpIndex].getUser();
        addTransaction(1, senderUser, receiver, amount, receiverIpIndex, accs);
    }
    addTransaction(0, senderUser, receiver, amount, senderIndex, accs);
    cout << "Transfer compeleted\n";
}

void depositAndWithdraw(int deposit, string &user, string &ip, long double &amount, vector<acc> &accs, const int &bankruptcy)
{
    int index = searchUser(user, accs);
    if (index == -1)
        throw invalid_argument("Error: User not found");
    int ipIndex = searchIP(ip, accs);
    if (ipIndex == -1 || ipIndex != index)
        throw invalid_argument("Error: IP not found");
    else if (amount <= 0)
        throw invalid_argument("Error: Invalid amount of money");
    else if (time(0) >= accs[index].getExpDate())
    {
        cout << "Your acc has expired\n";
        renewal(user, ip, accs);
        return;
    }
    if (deposit == 1)
    {
        accs[index].changeBalance(amount);
        addTransaction(1, "cash", user, amount, index, accs);
        cout << "Money successfully added. current balance is: " << accs[index].getBalance() << "$\n";
    }
    else if (deposit == 0)
    {
        if (accs[index].getDebt() > 0)
            throw invalid_argument("you have unpaid loan. you can't withdraw money");
        else if (accs[index].getBalance() < amount)
            throw invalid_argument("Your balance is not enough");
        else if (checkBankruptcy(amount, accs))
            throw invalid_argument("The bank went bankrupt. we can't pay the money");
        accs[index].changeBalance((-1) * amount);
        addTransaction(0, user, "cash", amount, index, accs);
        cout << "Money successfully withdrew. current balance is: " << accs[index].getBalance() << "$\n";
    }
}

void renewal(string &user, string &ip, vector<acc> &accs)
{
    int index = searchUser(user, accs);
    if (index == -1)
        throw invalid_argument("Error: User not found");
    int ipIndex = searchIP(ip, accs);
    if (ipIndex == -1 || ipIndex != index)
        throw invalid_argument("Error: IP not found");
    cout << "The cost of renwal is 2$. Do you want to renew your acc? (yes or no)\n";
    string decision;
    int correct = 0;
    while (correct == 0)
    {
        getline(cin, decision);
        if (decision == "yes")
        {
            correct = 1;
            if (accs[index].getBalance() < 2)
            {
                cout << "your acc balance is not enough\n";
                return;
            }
            else
            {
                accs[index].changeBalance(-2);
                accs[index].setExpDate();
                addTransaction(0, user, "Bank", 2, index, accs);
                cout << "Acc renewed successfully. New expire time is: ";
                time_t newExp = accs[index].getExpDate();
                tm *exp = localtime(&newExp);
                cout << (exp->tm_year + 1900) << '-' << (exp->tm_mon + 1) << '-' << (exp->tm_mday) << '\n';
            }
        }
        else if (decision == "no")
        {
            correct = 1;
            cout << "Renewal canceled\n";
            return;
        }
        else
            cout << "invalid command... Do you want to renew your acc? (yes or no)\n";
    }
}

void addIP(string &user, string &ip, vector<acc> &accs)
{
    int index = searchUser(user, accs);
    if (index == -1)
        throw invalid_argument("Error: User not found");
    else if (searchIP(ip, accs) != -1)
        throw invalid_argument("Error: Ip used before");
    else if (!ipValid(ip))
        throw invalid_argument("Error: IP is not valid");
    accs[index].setIP(ip);
    cout << "IP successfully added\n";
}

void addAcc(string &user, char *&ip, vector<acc> &accs)
{
    string strIP = ip;
    if (!userValid(user) || !ipValid(ip))
        throw invalid_argument("Error: invalid user or ip");
    int index = searchUser(user, accs);
    if (index > -1)
        throw invalid_argument("Error: User used before");
    else if (searchIP(ip, accs) != -1)
        throw invalid_argument("Error: IP used before");
    unsigned int CardNum = cardNum(accs);
    acc temp(user, strIP, CardNum);
    accs.push_back(temp);
    cout << "Acc successfully added\n";
}

void spltCmd(char *&command, vector<acc> &accs, const int &bankruptcy)
{
    char *token = strtok(command, " ");
    try
    {
        if (bankruptcy != 0)
            throw invalid_argument("bank has gone bankrupt. No services are provided");
        else if ((string)token == "create")
        {
            token = strtok(NULL, ":");
            string user = token;
            token = strtok(NULL, " ");
            if (token == NULL)
                throw invalid_argument("Error: invalid command");
            char *ip = token;
            addAcc(user, ip, accs);
        }
        else if ((string)token == "add_ip")
        {
            token = strtok(NULL, ":");
            string user = token;
            token = strtok(NULL, " ");
            if (token == NULL)
                throw invalid_argument("Error: invalid command");
            string ip = token;
            addIP(user, ip, accs);
        }
        else if ((string)token == "renewal")
        {
            token = strtok(NULL, ":");
            string user = token;
            token = strtok(NULL, " ");
            if (token == NULL)
                throw invalid_argument("Error: invalid command");
            string ip = token;
            renewal(user, ip, accs);
        }
        else if ((string)token == "deposit")
        {
            token = strtok(NULL, ":");
            string user = token;
            token = strtok(NULL, ":");
            string ip = token;
            token = strtok(NULL, " ");
            if (token == NULL)
                throw invalid_argument("Error: invalid command");
            string strAmount = token;
            long double amount = stold(strAmount);
            depositAndWithdraw(1, user, ip, amount, accs, bankruptcy);
        }
        else if ((string)token == "transfer")
        {
            token = strtok(NULL, ":");
            string senderUser = token;
            token = strtok(NULL, ":");
            string senderIP = token;
            token = strtok(NULL, ":");
            string receiverUser = token;
            token = strtok(NULL, " ");
            if (token == NULL)
                throw invalid_argument("Error: invalid command");
            string strAmount = token;
            long double amount = stold(strAmount);
            transfer(senderUser, senderIP, receiverUser, amount, accs);
        }
        else if ((string)token == "withdraw")
        {
            token = strtok(NULL, ":");
            string user = token;
            token = strtok(NULL, ":");
            string ip = token;
            token = strtok(NULL, " ");
            if (token == NULL)
                throw invalid_argument("Error: invalid command");
            string strAmount = token;
            long double amount = stold(strAmount);
            depositAndWithdraw(0, user, ip, amount, accs, bankruptcy);
        }
        else if ((string)token == "add_profit")
        {
            token = strtok(NULL, " ");
            if (token == NULL)
                throw invalid_argument("Error: invalid command");
            string user = token;
            addProfit(user, accs);
        }
        else if ((string)token == "get_loan")
        {
            token = strtok(NULL, ":");
            string user = token;
            token = strtok(NULL, ":");
            string ip = token;
            token = strtok(NULL, " ");
            if (token == NULL)
                throw invalid_argument("Error: invalid argument");
            string strAmount = token;
            long double amount = stold(strAmount);
            getLoan(user, ip, amount, accs);
        }
        else if ((string)token == "pay_loan")
        {
            token = strtok(NULL, ":");
            string user = token;
            token = strtok(NULL, " ");
            if (token == NULL)
                throw invalid_argument("Error: invalid command");
            string strAmount = token;
            long double amount = stold(strAmount);
            payLoan(user, amount, accs);
        }
        else if ((string)token == "bank")
        {
            cout << "Num of accs: " << accs.size() << '\n';
            long double balance = 0;
            long double loans = 0;
            for (int i = 0; i < accs.size(); i++)
            {
                balance += accs[i].getBalance();
                loans += accs[i].getDebt();
            }
            cout << "accs balances: " << balance << '\n';
            cout << "loans balances: " << loans << '\n';
        }
        else if ((string)token == "help")
        {
            cout << "defined commands:\n";
            cout << "open new acc    ---> create user:ip\n";
            cout << "adding new ip          ---> add_ip user:newIp\n";
            cout << "renewing acc       ---> renewal user:ip\n";
            cout << "deposit money          ---> deposit user:ip:amount\n";
            cout << "withdraw money         ---> withdraw user:ip:amount\n";
            cout << "transfering money      ---> transfer senderUser:senderIp:receiverUser:amount\n";
            cout << "adding acc profit ---> add_profit user\n";
            cout << "getting loan           ---> get_loan user:ip:amount\n";
            cout << "pay back the loan      ---> pay_loan user:ip:amount\n";
            cout << "print acc info     ---> user or ip or card num\n";
            cout << "print bank info        ---> bank\n";
        }
        else if (!print(token, accs))
            cout << "didint get it\n";
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}