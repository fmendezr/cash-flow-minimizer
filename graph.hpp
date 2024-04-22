#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>

using namespace std;

//this class includes all necessary attributes and methods needed for the adjacency matrix
class Graph
{
public:
    Graph();   
    
    void addVertex(); //add a new vertex (user) to the graph

    //update edges/balance between two vertices/users for a new transaction
    void addDebt(int creditorCode, int debtorCode, float amount); 

    map<int, float> getNetAmounts(); //map to store net amounts for each vertex/user

private:    
    vector<vector<float>> matrix; //stores the adjacency matrix of the graph

    int size; //keeps track of the number of users/vertices
};

#endif

