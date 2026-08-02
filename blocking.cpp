#include "gemm.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void blocckingMatMul(const vector<vector<int>> &mat1, const vector<vector<int>> &mat2, vector<vector<int>> &ans, int blockSize) {
    int row1 = mat1.size();
    int col1 = mat1[0].size();
    int row2 = mat2.size();
    int col2 = mat2[0].size();

    if (col1 != row2) {
        cout << "Invalid input dimensions for matrix multiplication." << endl;
        return;
    }

    for (int i = 0; i < row1; i += blockSize) {
        for (int j = 0; j < col2; j += blockSize) {
            for (int k = 0; k < col1; k += blockSize) {
                // Compute the block multiplication
                for (int ii = i; ii < min(i + blockSize, row1); ++ii) {
                    for (int jj = j; jj < min(j + blockSize, col2); ++jj) {
                        for (int kk = k; kk < min(k + blockSize, col1); ++kk) {
                            ans[ii][jj] += mat1[ii][kk] * mat2[kk][jj];
                        }
                    }
                }
            }
        }
    }
}