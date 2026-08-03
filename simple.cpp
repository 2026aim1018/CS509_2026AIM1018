#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

void simpleMatMul(const vector<vector<int>> &mat1,const vector<vector<int>> &mat2, vector<vector<int>> &ans){

    if(mat1.empty() || mat2.empty() || mat1.size() != mat2[0].size()){
        cout<<"Error: Invalid dimensions for matrix multiplication." << endl;
        return;
    }
    
    int row1 = mat1.size();
    int col1 = mat1[0].size();

    int row2 = mat2.size();
    int col2 = mat2[0].size();

    if(col1 != row2){
        cout<<"Invalid i/p"<< endl;
        return;
    }
    for(int i=0; i<row1; i++){
        for(int j=0; j<col2; j++){
            for(int k=0; k<col1; k++){
                ans[i][j] += mat1[i][k]*mat2[k][j];
            }
        }
    }
}
