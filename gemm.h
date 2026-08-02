#ifndef GEMM_H
#define GEMM_H

#include <vector>
#include <iostream>
using namespace std;

void simpleMatMul(const vector<vector<int>> &mat1, const vector<vector<int>> &mat2, vector<vector<int>> &ans);
void blockingMatMul(const vector<vector<int>> &mat1, const vector<vector<int>> &mat2, vector<vector<int>> &ans, int blockSize);

#endif

