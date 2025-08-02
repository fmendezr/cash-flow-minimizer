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

    //initialize net balances for all people
    for (int i = 0; i < numPeople; i++)
    {
        netBalances[i] = 0.0f; //initialize net balance to 0
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
        int newPersonCode = persons.size();
        persons[name] = newPersonCode;
        netBalances[newPersonCode] = 0.0f; //initialize net balance for new person
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

    //build debtor codes list (everyone except creditor)
    for (pair<string, int> person : persons)
    {
        int debtorCode = person.second;
        if (creditorCode == debtorCode) continue;
        debtorCodes.push_back(debtorCode);
    }

    //update net balances
    updateNetBalances(creditorCode, debtorCodes, amount);

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

    //update net balances
    vector<int> debtorCodes;
    debtorCodes.push_back(debtorCode);
    updateNetBalances(creditorCode, debtorCodes, amount);

    //create receipt
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

    //update net balances
    updateNetBalances(creditorCode, debtorCodes, amount);

    //create receipt
    ReceiptLog.push_back(Receipt(debtorCodes, creditorCode, amount, message));
}

//update net balances for a transaction
void CashFlowMinimizer::updateNetBalances(int creditorCode, vector<int> debtorCodes, float amount)
{
    // creditor gets money (positive balance)
    netBalances[creditorCode] += amount * debtorCodes.size();
    
    // debtors lose money (negative balance)
    for (int debtorCode : debtorCodes)
    {
        if (creditorCode != debtorCode) // avoid self-payment
        {
            netBalances[debtorCode] -= amount;
        }
    }
}

//find minimum transactions
void CashFlowMinimizer::minimizeTransactions()
{
    // Convert netBalances to vector for sorting
    vector<pair<int, float>> balances;
    for (const auto& balance : netBalances)
    {
        if (abs(balance.second) > 0.01f) // Only include non-zero balances (with small tolerance for floating point)
        {
            balances.push_back(balance);
        }
    }
    
    // Sort by balance amount (ascending)
    sort(balances.begin(), balances.end(), 
         [](const pair<int, float>& a, const pair<int, float>& b) {
             return a.second < b.second;
         });
    
    int left = 0;  // Index for debtors (negative balances)
    int right = balances.size() - 1;  // Index for creditors (positive balances)
    
    while (left < right)
    {
        float leftBalance = balances[left].second;
        float rightBalance = balances[right].second;
                
        // If both are debtors or both are creditors, we can't match  (shouldn't happen ever, but just in case)
        if ((leftBalance < 0 && rightBalance < 0) || (leftBalance > 0 && rightBalance > 0))
        {
            break;
        }
        
        float transactionAmount;
        int debtorCode, creditorCode;
        
        if (leftBalance < 0 && rightBalance > 0)
        {
            // Normal case: debtor (left) pays creditor (right)
            debtorCode = balances[left].first;
            creditorCode = balances[right].first;
            transactionAmount = min(abs(leftBalance), rightBalance);
            
            // Update balances
            balances[left].second += transactionAmount;
            balances[right].second -= transactionAmount;
        }
        else
        {
            // Reverse case: creditor (left) pays debtor (right)
            creditorCode = balances[left].first;
            debtorCode = balances[right].first;
            transactionAmount = min(leftBalance, abs(rightBalance));
            
            // Update balances
            balances[left].second -= transactionAmount;
            balances[right].second += transactionAmount;
        }
        
        // Add transaction
        transactionLog.push_back(Transaction(debtorCode, creditorCode, transactionAmount));
        
        // Remove settled balances
        if (abs(balances[left].second) < 0.01f) left++;
        if (abs(balances[right].second) < 0.01f) right--;
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