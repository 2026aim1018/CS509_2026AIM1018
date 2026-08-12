#ifndef BELLMANFORD_H
#define BELLMANFORD_H

#include <iostream>
#include <vector>
using namespace std;

void funcBellmanFord(int V, vector<int> &rowPtr, vector<int> &colIdx, int sourceNode, vector<int> &dis, vector<int> &weights, bool &hasNegCycle);

#endif 