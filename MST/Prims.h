#ifndef PRIMS_H
#define PRIMS_H

#include<vector>
#include "MSTEdge.h"
using namespace std;

long long primsAlgoFunc(int V, const vector<int> &rowPtr, const vector<int> &colIdx, const vector<int> &weights, vector<MSTEdge> &mstEdges);

#endif

