#ifndef PRIMS_H
#define PRIMS_H

#include<vector>
using namespace std;

struct MSTEdge{
    int sourceNode;
    int destNode;
    int weight;
};

long long primsAlgoFunc(int V, const vector<int> &rowPtr, const vector<int> &colIdx, const vector<int> &weights, const vector<MSTEdge> &mstEdges);

#endif

