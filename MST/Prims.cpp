#include "Prims.h"
#include <queue>
#include <vector>

using namespace std;

struct EdgeToProcess{
    int weight;
    int sourceNode;
    int destNode;

    bool operator>(const EdgeToProcess& other) const{
        return weight > other.weight;
    }
};

long long primsAlgoFunc(int V, const vector<int> &rowPtr,const vector<int> &colIdx, const vector<int> &weights, vector<MSTEdge> &mstEdges){
    long long totalTreeWeight = 0;

    vector<bool> isNodeInTree(V, false);

    priority_queue<EdgeToProcess, vector<EdgeToProcess>, greater<EdgeToProcess>> minHeap;

    int startNode = 0;
    isNodeInTree[startNode] = true;

    for(int i=rowPtr[startNode]; i<rowPtr[startNode+1]; i++){
        minHeap.push({weights[i], startNode, colIdx[i]});
    }

    while(!minHeap.empty() && mstEdges.size()<V-1){
        EdgeToProcess currEdge = minHeap.top();
        minHeap.pop();

        int u = currEdge.sourceNode;
        int v = currEdge.destNode;
        int currWeight = currEdge.weight;

        if(isNodeInTree[v] == true){
            continue;
        }
        mstEdges.push_back({u, v, currWeight});
        totalTreeWeight += currWeight;
        isNodeInTree[v] = true; 

        for(int i=rowPtr[v]; i<rowPtr[v+1]; i++){
            int nxtNode = colIdx[i];
            int nxtWeight = weights[i];

            if(isNodeInTree[nxtNode]==false){
                minHeap.push({nxtWeight, v, nxtNode});
            }
        }
    }
    return totalTreeWeight;

}