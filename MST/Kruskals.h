#ifndef KRUSKALS_H
#define KRUSKALS_H

#include <vector>
#include "MSTEdge.h"
using namespace std;

long long kruskalsAlgoFunc(int V, const vector<int> &rowPtr, const vector<int> &colIdx, const vector<int> &weights, vector<MSTEdge> &mstEdges);
#endif