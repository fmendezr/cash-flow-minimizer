#include <iostream>
#include <vector>
#include "cashFlowMinimizer.hpp"
#include <cmath>

using namespace std;

CashFlowMinimizer::CashFlowMinimizer() : numPeople(0), endInput(false)
{
    gameLoop();
}

//main program
void CashFlowMinimizer::gameLoop()
{
    getNumberPeople(); //retrieve number of people involved in transactions

    //instantiate adjacency matrix
    for (int i = 0; i < numPeople; i++)
    {
        graph.addVertex();
        getPersonInfo(i); //retrieve names of people
    }
    
    while (!endInput)
    {
        char answer;
        showInstructions(); //display menu
        cin >> answer;

        switch (answer)
        {
        case 'a': //split between all users
            handleSplitAll();
            break;

        case 'b': //one to one transaction
            handleIndividual();
            break;
        
        case 'c': //split between n number of users
            handleSplitSubgroup();
            break;

        case 'd': //end input
            endInput = true;
            minimizeTransactions(); //find minimum transactions
            giveFinalOutput();      
            break;
        
        default:
            cout << "Invalid input. Please try again." << endl;
        }
    }
}

//display menu
void CashFlowMinimizer::showInstructions()
{
    cout << "Enter the letter for the option you want to select:\n\t(a) Split between everyone.\n\t(b) For an individual.\n\t(c) Split betweem multiple individuals (not everyone).\n\t(d) Get results.\n";   
}

//retrieve number of people involved in transactions
void CashFlowMinimizer::getNumberPeople()
{
    cout << "How many people are in your trip: ";
    cin >> numPeople;
}

//retrieve names of people
void CashFlowMinimizer::getPersonInfo(int n)
{
    string name;
    cout << "Enter name of person " << n + 1 << ": ";
    cin >> name;
    persons[name] = n;
}

//verify unrecognized input
bool CashFlowMinimizer::verifyUser(string name){
    string userinput;
    cout << "Person was not in trip. Do you want to add them to the group and record this transaction? Enter Y for 'yes', N for 'no'" << endl;
    cin >> userinput;

    if (userinput == "Y"){
        graph.addVertex();
        persons[name] = persons.size();
        numPeople++;
        return true;
    }

    else return false;
}

//split between all users
void CashFlowMinimizer::handleSplitAll()
{
    string creditorName;
    cout << "Enter the creditor's name: " ;
    cin >> creditorName;

    //unrecognized creditor input
    if (!persons.count(creditorName))
    {
        bool addUser = verifyUser(creditorName);
        if (!addUser) return;
    }

    float amount;
    cout << "Enter the amount: " ;
    cin >> amount;
    amount /= numPeople; //split evenly

    string message;
    cout << "Add the reason for this payment: ";
    cin >> message;

    int creditorCode = persons[creditorName]; //retrieve code from map
    
    vector<int> debtorCodes;

    //update matrix
    for (pair<string, int> person : persons)
    {
        int debtorCode = person.second;
        if (creditorCode == debtorCode) continue;
        debtorCodes.push_back(debtorCode);
        graph.addDebt(creditorCode, debtorCode, amount);
    }

    //create receipt
    ReceiptLog.push_back(Receipt(debtorCodes, creditorCode, amount, message));
}

//one to one transaction
void CashFlowMinimizer::handleIndividual()
{
    string creditorName;
    cout << "Enter the creditor's name: " ;
    cin >> creditorName;

    //unrecognized creditor input
    if (!persons.count(creditorName))
    {
        bool addUser = verifyUser(creditorName);
        if (!addUser) return;
    }

    string debtorName;
    cout << "Enter the debtor's name: " ;
    cin >> debtorName;

    //unrecognized debtor input
    if (!persons.count(debtorName))
    {
        bool addUser = verifyUser(debtorName);
        if (!addUser) return;
    }

    //retreive codes from map
    int creditorCode = persons[creditorName];
    int debtorCode = persons[debtorName];

    float amount;
    cout << "Enter the amount: " ;
    cin >> amount;

    string message;
    cout << "Add the reason for this payment: ";
    cin >> message;

    //update matrix
    graph.addDebt(creditorCode, debtorCode, amount);

    //create receipt
    vector<int> debtorCodes;
    debtorCodes.push_back(debtorCode);
    ReceiptLog.push_back(Receipt(debtorCodes, creditorCode, amount, message));
}

//split between n number of users
void CashFlowMinimizer::handleSplitSubgroup()
{
    string creditorName;
    cout << "Enter the creditor's name: " ;
    cin >> creditorName;

    //unrecognized creditor input
    if (!persons.count(creditorName))
    {
        bool addUser = verifyUser(creditorName);
        if (!addUser) return;
    }

    //retrieve code from map
    int creditorCode = persons[creditorName];

    vector<int> debtorCodes; //store all debtors

    while (true)
    {
        string debtorName;
        cout << "Enter the debtor's name (if no more debtors, enter END): ";
        cin >> debtorName;
        if (debtorName == "END" ) break;

        //unrecognized debtor input
        if (!persons.count(debtorName))
        {
            bool addUser = verifyUser(debtorName);
            if (!addUser) continue;
        }

        debtorCodes.push_back(persons[debtorName]);
    }

    float amount;
    cout << "Enter the amount: " ;
    cin >> amount;
    amount /= debtorCodes.size()+1; //split evenly

    string message;
    cout << "Add the reason for this payment: ";
    cin >> message;

    //update matrix
    for (int debtorCode: debtorCodes)
    {
        if (creditorCode == debtorCode) continue;
        graph.addDebt(creditorCode, debtorCode, amount);
    }

    //create receipt
    ReceiptLog.push_back(Receipt(debtorCodes, creditorCode, amount, message));
}

//find minimum transactions
void CashFlowMinimizer::minimizeTransactions()
{
    map<int, float> netAmounts = graph.getNetAmounts();

    while (!netAmounts.empty())
    {
        //initialize
        pair<int, float> minPair = {0, 1e10};
        pair<int, float> maxPair = {0, -1e10};

        //find minimum and maximum net amounts
        for (auto &currPair : netAmounts)
        {
            if (currPair.second < minPair.second) minPair = currPair;
            if (currPair.second > maxPair.second) maxPair = currPair;
        }

        int transactionAmount;

        //if absolute value of max and min are equal, minimum (creditor) pays maximum (debtor) their balances
        if (abs(minPair.second) == abs(maxPair.second))
        {
            transactionAmount = maxPair.second;
            transactionLog.push_back(Transaction(minPair.first, maxPair.first, transactionAmount));

            //remove users from netamounts map
            netAmounts.erase(minPair.first);
            netAmounts.erase(maxPair.first);
        }

        //if min greater than max, minimum (creditor) pays maximum (debtor) the maximum balance
        else if (abs(minPair.second) > abs(maxPair.second))
        {   
            transactionAmount = maxPair.second;
            netAmounts[minPair.first] += maxPair.second; //update net amount balance
            transactionLog.push_back(Transaction(minPair.first, maxPair.first, transactionAmount));

            //remove max user from netamounts map
            netAmounts.erase(maxPair.first);
        }

        //if max greater than min, minimum (creditor) pays maximum (debtor) the minimum balance
        else 
        {
            transactionAmount = abs(minPair.second);
            netAmounts[maxPair.first] += minPair.second; //update net amount balance
            transactionLog.push_back(Transaction(minPair.first, maxPair.first, transactionAmount));

            //remove min user from netamounts map
            netAmounts.erase(minPair.first);
        }
    }
}

//display output
void CashFlowMinimizer::giveFinalOutput()
{
    
    cout << "ALL TRANSACTIONS ENTERED" << endl;

    for (auto transaction : ReceiptLog){
        string debtor, creditor;
        for (const auto& pair : persons){
            if (pair.second == transaction.creditorCode) {
                creditor = pair.first;
                break;
            }
        }

        for (const auto& pair : persons){
            for (int user : transaction.debtorCode){
                if (transaction.debtorCode.size() > 1){
                    if (pair.second == user){
                        debtor = pair.first;   
                        if (pair.second == transaction.debtorCode.back()){
                            cout << "and " << debtor << " ";
                        }
                        else cout << debtor << (transaction.debtorCode.size() == 2 ? " " : ", ");
                    }
                }
                else{
                    if (pair.second == user){
                        debtor = pair.first;
                        cout << debtor << " has to pay " << creditor << " $" << transaction.amount << " for " << transaction.description << endl; 
                    }
                }
            }
        }
        if (transaction.debtorCode.size() > 1){
                cout << "has to pay " << creditor << " $" << transaction.amount << " for " << transaction.description << endl;
        }
    }

    cout << endl;
    cout << "MINIMUM TRANSACTIONS NEEDED TO PAY OFF ALL BALANCES" << endl;

    for (auto transaction : transactionLog)
    {
        string debtor1, creditor1;
        for (const auto& pair : persons){
            if (pair.second == transaction.creditorCode) {
                creditor1 = pair.first;
                continue;
            }
            if (pair.second == transaction.debtorCode) debtor1 = pair.first; 
        }
        if (transaction.amount) cout << debtor1 << " has to pay " << creditor1 << " $" << transaction.amount << endl;
    }
}