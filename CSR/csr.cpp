#include "csr.h"
#include <iostream>
using namespace std;

void convertToCSR_Unweighted(const vector<vector<int>> &adjList, vector<int> &rowPtr, vector<int> &colIdx) {
    int V = adjList.size(); // Number of vertices
    rowPtr.assign(V+1, 0);
    colIdx.clear();

    int currentEdgeCount = 0;

    for(int i=0; i<V; i++){
        rowPtr[i] = currentEdgeCount;

        for(int neighbor : adjList[i]){
            colIdx.push_back(neighbor);
            currentEdgeCount++;
        }
    }
    rowPtr[V] = currentEdgeCount;
}

void convertToCSR_Weighted(const vector<vector<pair<int, int>>> &adjList, vector<int> &rowPtr, vector<int> &colIdx, vector<int> &weights){
    int V = adjList.size(); // Number of vertices
    rowPtr.assign(V+1, 0);
    colIdx.clear();
    weights.clear();

    int currentEdgeCount = 0;

    for(int i=0; i<V; i++){
        rowPtr[i] = currentEdgeCount;

        for(auto edge: adjList[i]){
            colIdx.push_back(edge.first);
            weights.push_back(edge.second);
            currentEdgeCount++;
        }
    }
    rowPtr[V] = currentEdgeCount;
}