#include <iostream>
#include <string>
#include <map>
#include <cmath>

using namespace std;

class Transaction
{
public:
    int creditorCode;
    int debtorCode;
    float amount;

    Transaction(int d, int c, float a) : debtorCode(d), creditorCode(c), amount(a) {}
};

void printInstructions()
{
    cout << "Enter the letter for the option you want to select:\n\t(a) Split between everyone.\n\t(b) For an individual.\n\t(c) Split betweem multiple individuals (not everyone).\n\t(d) Get results.\n";   
}

void getResults(vector<vector<float>>& graphMatrix)
{
    
    map<int, float> netAmounts;
    for (int i = 0; i < graphMatrix.size(); i++)
    {
        netAmounts[i] = 0;
        for (int j = 0; j < graphMatrix.size(); j++)
            netAmounts[i] += graphMatrix[i][j] - graphMatrix[j][i];
    }

    vector<Transaction> transaction_log;

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
            transaction_log.push_back(Transaction(minPair.first, maxPair.first, transactionAmount));
            netAmounts.erase(minPair.first);
            netAmounts.erase(maxPair.first);
        }
        else if (abs(minPair.second) > abs(maxPair.second))
        {   
            transactionAmount = maxPair.second;
            netAmounts[minPair.first] += maxPair.second;
            transaction_log.push_back(Transaction(minPair.first, maxPair.first, transactionAmount));
            netAmounts.erase(maxPair.first);
        }
        else 
        {
            transactionAmount = abs(minPair.second);
            netAmounts[maxPair.first] += minPair.second;
            transaction_log.push_back(Transaction(minPair.first, maxPair.first, transactionAmount));
            netAmounts.erase(minPair.first);
        }
    }

    
    for (auto transaction : transaction_log)
    {
        cout << transaction.debtorCode << " has to pay " << transaction.creditorCode << " $" << transaction.amount << endl;
    }
    
}

int main()
{
    int num_people;
    cout << "How many people are in your trip: ";
    cin >> num_people;

    map<string, int> persons;
    vector<vector<float>> graphMatrix;

    for (int i = 0; i < num_people; i++)
        graphMatrix.push_back(vector<float>(num_people, 0));

    for (int n = 0 ; n < num_people; n++)
    {
        string name;
        cout << "Enter name of person " << n + 1 << ": ";
        cin >> name;
        persons[name] = n;
    }

    bool end = false;
    while (!end)
    {
        char answer;
        printInstructions();
        cin >> answer;
        switch (answer)
        {
        case 'a':
        {
            string creditorName;
            cout << "Enter the creditor's name: " ;
            cin >> creditorName;

            if (!persons.count(creditorName))
            {
                cout << "Person was not in trip." << endl;
                break;
            }

            float amount;
            cout << "Enter the amount: " ;
            cin >> amount;
            amount /= num_people;

            int creditorCode = persons[creditorName];

            for (pair<string, int> person : persons )
            {
                int debtorCode = person.second;
                if (creditorCode == debtorCode) continue;
                graphMatrix[creditorCode][debtorCode] += amount;
            }

            break;
        }

        case 'b':
        {
            string creditorName;
            cout << "Enter the creditor's name: " ;
            cin >> creditorName;

            if (!persons.count(creditorName))
            {
                cout << "Person was not in trip." << endl;
                break;
            }

            string debtorName;
            cout << "Enter the debtor's name: " ;
            cin >> debtorName;

            if (!persons.count(debtorName))
            {
                cout << "Person was not in trip." << endl;
                break;
            }

            int creditorCode = persons[creditorName];
            int debtorCode = persons[debtorName];

            float amount;
            cout << "Enter the amount: " ;
            cin >> amount;
            
            graphMatrix[creditorCode][debtorCode] += amount;

            break;
        }
        
        case 'c':
        {
            string creditorName;
            cout << "Enter the creditor's name: " ;
            cin >> creditorName;

            if (!persons.count(creditorName))
            {
                cout << "Person was not in trip." << endl;
                break;
            }

            int creditorCode = persons[creditorName];

            vector<int> debtorsCodes;

            while (true)
            {
                string debtorName;
                cout << "Enter the debtor's name (if no more debtors, enter END): " ;
                cin >> debtorName;

                if (debtorName == "END" ) break;
 
                if (!persons.count(debtorName))
                {
                    cout << "Person was not in trip." << endl;
                    break;
                }

                debtorsCodes.push_back(persons[debtorName]);
            }

            float amount;
            cout << "Enter the amount: " ;
            cin >> amount;

            for (int debtorCode: debtorsCodes)
            {
                if (creditorCode == debtorCode) continue;
                graphMatrix[creditorCode][debtorCode] += amount;
            }
            break;
            
        }
        
        case 'd':
            getResults(graphMatrix);
            end = true;
            break;
        
        default:
            cout << "Invalid input. Please try again." << endl;
        }
    } 


    return 0;
}