#ifndef CASHFLOWMINIMIZER_H
#define CASHFLOWMINIMIZER_H

#include <map>
#include <string>
#include <iostream>
#include "graph.hpp"

using namespace std;

class Transaction
{
public:
    int creditorCode;
    int debtorCode;
    float amount;

    Transaction(int d, int c, float a) : debtorCode(d), creditorCode(c), amount(a) {}
};

class Receipt
{
    public: 
        string description;
        int creditorCode;
        vector<int> debtorCode;
        float amount;
};

class CashFlowMinimizer
{
public:
    CashFlowMinimizer();

    void gameLoop();
    
private:
   int numPeople;
    map<string, int> persons;
    vector<Transaction> transactionLog;
    Graph graph;
    bool endInput;
    
    void showInstructions();

    void getNumberPeople();

    void getPersonInfo(int n);

    void handleSplitAll();

    void handleIndividual();

    void handleSplitSubgroup();

    void minimizeTransactions();

    void giveFinalOutput();
};

#endif 