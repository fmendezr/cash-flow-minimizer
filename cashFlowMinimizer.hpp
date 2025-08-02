#ifndef CASHFLOWMINIMIZER_H
#define CASHFLOWMINIMIZER_H

#include <map>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

//this class includes all information of each transaction input
class Receipt
{
    public: 
        vector<int> debtorCode; //codes to represent debtors
        int creditorCode;       //code to represent creditor
        float amount;
        string description;     //reason of transaction

        Receipt(vector<int> d, int c, float a, string des) : debtorCode(d), creditorCode(c), amount(a), description(des) {} 
};

//this class includes all information of each minimized transactions
class Transaction
{
public: 
    int debtorCode;
    int creditorCode;
    float amount; 

    Transaction(int d, int c, float a) : debtorCode(d), creditorCode(c), amount(a) {}
};

//this class includes all attributes and methods required for the minimizer
class CashFlowMinimizer
{
public:
    CashFlowMinimizer();

    void gameLoop(); //repeatedly takes input for each transaction
    
private:
    int numPeople;                      //number of users involved
    map<string, int> persons;           //map to store individual's names and corresponding code
    vector<Receipt> ReceiptLog;         //vector of all input transactions
    vector<Transaction> transactionLog; //vector of minized transactions
    map<int, float> netBalances;        //map to store net balance for each person (personCode -> netBalance)
    bool endInput;                      //control for loop
    
    //helper functions
    void showInstructions();        //display menu

    void getNumberPeople();         //retrieve number of people involved in transactions

    void getPersonInfo(int n);      //retrieve names of people

    bool verifyUser(string name);   //verify unrecognized input

    void handleSplitAll();          //split between all users

    void handleIndividual();        //one to one transaction

    void handleSplitSubgroup();     //split between n number of users

    void minimizeTransactions();    //find minimum transactions

    void giveFinalOutput();         //display output
    
    void updateNetBalances(int creditorCode, vector<int> debtorCodes, float amount); //update net balances for a transaction
};

#endif 