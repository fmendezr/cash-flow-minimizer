#include <iostream>
#include <vector>
#include "cashFlowMinimizer.hpp"
#include <cmath>

using namespace std;

CashFlowMinimizer::CashFlowMinimizer() : numPeople(0), endInput(false)
{
    gameLoop();
}

void CashFlowMinimizer::gameLoop()
{
    getNumberPeople();

    for (int i = 0; i < numPeople; i++)
    {
        graph.addVertex();
        getPersonInfo(i);
    }
    
    while (!endInput)
    {
        char answer;
        showInstructions();
        cin >> answer;

        switch (answer)
        {
        case 'a':
            handleSplitAll();
            break;

        case 'b':
            handleIndividual();
            break;
        
        case 'c':
            handleSplitSubgroup();
            break;

        case 'd':
            endInput = true;
            minimizeTransactions();
            giveFinalOutput();
            break;
        
        default:
            cout << "Invalid input. Please try again." << endl;
        }
    }
}

void CashFlowMinimizer::showInstructions()
{
    cout << "Enter the letter for the option you want to select:\n\t(a) Split between everyone.\n\t(b) For an individual.\n\t(c) Split betweem multiple individuals (not everyone).\n\t(d) Get results.\n";   
}

void CashFlowMinimizer::getNumberPeople()
{
    cout << "How many people are in your trip: ";
    cin >> numPeople;
}

void CashFlowMinimizer::getPersonInfo(int n)
{
    string name;
    cout << "Enter name of person " << n + 1 << ": ";
    cin >> name;
    persons[name] = n;
}

void CashFlowMinimizer::handleSplitAll()
{
    string creditorName;
        cout << "Enter the creditor's name: " ;
        cin >> creditorName;

        if (!persons.count(creditorName))
        {
            cout << "Person was not in trip." << endl;
            return;
        }

        float amount;
        cout << "Enter the amount: " ;
        cin >> amount;
        amount /= numPeople;

        int creditorCode = persons[creditorName];
        
        for (pair<string, int> person : persons )
        {
            int debtorCode = person.second;
            graph.addDebt(creditorCode, debtorCode, amount);
        }
}

void CashFlowMinimizer::handleIndividual()
{
    string creditorName;
    cout << "Enter the creditor's name: " ;
    cin >> creditorName;

    if (!persons.count(creditorName))
    {
        cout << "Person was not in trip." << endl;
        return;
    }

    string debtorName;
    cout << "Enter the debtor's name: " ;
    cin >> debtorName;

    if (!persons.count(debtorName))
    {
        cout << "Person was not in trip." << endl;
        return;
    }

    int creditorCode = persons[creditorName];
    int debtorCode = persons[debtorName];

    float amount;
    cout << "Enter the amount: " ;
    cin >> amount;
    
    graph.addDebt(creditorCode, debtorCode, amount);
}

void CashFlowMinimizer::handleSplitSubgroup()
{
    string creditorName;
    cout << "Enter the creditor's name: " ;
    cin >> creditorName;

    if (!persons.count(creditorName))
    {
        cout << "Person was not in trip." << endl;
    }

    int creditorCode = persons[creditorName];

    vector<int> debtorsCodes;
    while (true)
    {
        string debtorName;
        do 
        {
            cout << "Enter the debtor's name (if no more debtors, enter END): " ;
            cin >> debtorName;
            if (debtorName == "END" ) break;
            if (!persons.count(debtorName))
            {
                string userinput;
                cout << "Person was not in trip. Do you want to add them to the group? Enter Y for 'yes', N for 'no'" << endl;
                cin >> userinput;

                if (userinput == "Y"){
                    graph.addVertex();
                    persons[debtorName] = persons.size();
                    numPeople++;
                }
                else continue;
            }
            debtorsCodes.push_back(persons[debtorName]);
            break;
        } while (true);
    }

    float amount;
    cout << "Enter the amount: " ;
    cin >> amount;

    for (int debtorCode: debtorsCodes)
    {
        if (creditorCode == debtorCode) continue;
        graph.addDebt(creditorCode, debtorCode, amount);
    }
}

void CashFlowMinimizer::minimizeTransactions()
{
    map<int, float> netAmounts = graph.getNetAmounts();

    while (!netAmounts.empty())
    {
        cout << endl;
        for (const auto& pair : netAmounts) 
        {
        std::cout << pair.first << " - " << pair.second << std::endl;
        }
        cout << endl;

        pair<int, float> minPair = {0, 1e10};
        pair<int, float> maxPair = {0, -1e10};

        for (auto currPair : netAmounts)
        {
            if (currPair.second < minPair.second) minPair = currPair;
            if (currPair.second > maxPair.second) maxPair = currPair;
        }

        cout << minPair.first << " " << minPair.second << endl;
        cout << maxPair.first << " " << maxPair.second << endl;

        cout << endl;

        int transactionAmount;
        if (abs(minPair.second) == abs(maxPair.second))
        {
            transactionAmount = maxPair.second;
            transactionLog.push_back(Transaction(minPair.first, maxPair.first, transactionAmount));
            netAmounts.erase(minPair.first);
            netAmounts.erase(maxPair.first);
        }
        else if (abs(minPair.second) > abs(maxPair.second))
        {   
            transactionAmount = maxPair.second;
            netAmounts[minPair.first] += maxPair.second;
            transactionLog.push_back(Transaction(minPair.first, maxPair.first, transactionAmount));
            netAmounts.erase(maxPair.first);
        }
        else 
        {
            transactionAmount = abs(minPair.second);
            netAmounts[maxPair.first] += minPair.second;
            transactionLog.push_back(Transaction(minPair.first, maxPair.first, transactionAmount));
            netAmounts.erase(minPair.first);
        }
    }
}

void CashFlowMinimizer::giveFinalOutput()
{
    

    for (auto transaction : transactionLog)
    {
        cout << transaction.debtorCode << " has to pay " << transaction.creditorCode << " $" << transaction.amount << endl;
    }
}