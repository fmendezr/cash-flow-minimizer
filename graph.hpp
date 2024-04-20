#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>

using namespace std;

class Graph
{
public:
    Graph();   
    
    void addVertex();

    void addDebt(int creditorCode, int debtorCode, int amount);

    map<int, float> getNetAmounts();

private:    
    vector<vector<float>> matrix;

    int size;
};

#endif

