#include "graph.hpp"
#include <iostream>

using namespace std;

Graph::Graph() : size(0) {};

//add a new vertex (user) to the graph
void Graph::addVertex()
{
    size++;
    if (size)
    {
        for (vector<float>& vec : matrix){
            vec.push_back(0);
        }
    }
    matrix.push_back(vector<float>(size, 0));
}

//update edges/balance between two vertices/users for a new transaction
void Graph::addDebt(int creditorCode, int debtorCode, float amount)
{
    if (creditorCode == debtorCode) return;
    matrix[creditorCode][debtorCode] += amount;
}

//retrieve net amounts for each vertex/user
map<int, float> Graph::getNetAmounts()
{
    map<int, float> netAmounts;
    for (int i = 0; i < size; i++)
    {
        netAmounts[i] = 0;
        for (int j = 0; j < size; j++){
            netAmounts[i] += matrix[i][j] - matrix[j][i]; 
        }  
    }
    return netAmounts;
}