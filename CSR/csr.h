#ifndef CSR_H
#define CSR_H

#include<vector>
using namespace std;

void convertToCSR_Unweighted(const vector<vector<int>> &adjList, vector<int> &rowPtr, vector<int> &colIdx);
void convertToCSR_Weighted(const vector<vector<pair<int, int>>> &adjList, vector<int> &rowptr, vector<int> &colIdx, vector<int> &weights);

#endif
