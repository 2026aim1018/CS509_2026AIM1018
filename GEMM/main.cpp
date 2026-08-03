#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "gemm.h"
#include "csr.h"

using namespace std;

void printMat(const vector<vector<int>> &mat){
    for(int i=0; i<mat.size(); i++){
        for(int j=0; j<mat[0].size(); j++){
            cout<<mat[i][j]<<" ";
        }
        cout<<"\n";
    }
}

void readAndConvertToCSR(string fileName){
    ifstream inputFile(fileName);

    if(!inputFile.is_open()){
        cout<<"Error: Could not open file"<<fileName<<endl;
        return;
    }

    int V, E, isWeighted;

    if(!(inputFile>>V>>E>>isWeighted)){
        cout<<"Error: Invalid or empty file."<<endl;
        return;
    }

    if(isWeighted==0){
        vector<vector<int>> adjList(V);
        for(int i=0; i<E; i++){
            int u, v;
            inputFile>>u>>v;
            adjList[u].push_back(v);
        }

        vector<int> rowPtr, colIdx;
        convertToCSR_Unweighted(adjList, rowPtr, colIdx);
    }
    else{
        vector<vector<pair<int, int>>> adjList(V);
        
        for(int i=0; i<E; i++){
            int u, v, weight;
            inputFile>>u >> v >> weight;
            adjList[u].push_back(make_pair(v, weight));
        }

        vector<int> rowPtr, colIdx, weights;
        convertToCSR_Weighted(adjList, rowPtr, colIdx, weights);
    }
    inputFile.close();
}

int main(int argc, char* argv[]){

    if (argc < 2) {
        cout << "Error: Missing input file.\n";
        cout << "Usage: ./gemm_program <input_filename.txt>\n";
        return 1; // Exit with an error
    }
    string fileName = argv[1];
    ifstream inputFile(fileName);

    if(!inputFile.is_open()){
        cout<<"Error: Could not open file"<<fileName<<endl;
        return 1;
    }

    int M, K, N;
    inputFile >> M >> K >> N;

    vector<vector<int>> mat1(M, vector<int>(K, 0));
    vector<vector<int>> mat2(K, vector<int>(N, 0));
    vector<vector<int>> ansSimple(M, vector<int>(N, 0));
    vector<vector<int>> ansBlocking(M, vector<int>(N, 0));

    for(int i=0; i<M; i++){
        for(int j=0; j<K; j++){
            inputFile >> mat1[i][j];
        }
    }

    for(int i=0; i<K; i++){
        for(int j=0; j<N; j++){
            inputFile >> mat2[i][j];
        }
    }
    inputFile.close();

    if(mat1.empty() || mat2.empty() || mat1[0].size() != mat2.size()){
        cout<<"Error: Invalid dimensions for matrix multiplication." << endl;
        return 1;
    }

    auto startSimple = chrono::high_resolution_clock::now();
    simpleMatMul(mat1, mat2, ansSimple);
    auto endSimple = chrono::high_resolution_clock::now();
    auto timeSimple = chrono::duration_cast<chrono::milliseconds>(endSimple - startSimple).count();

    cout << "Algorithm: GEMM Simple\n" << endl;
    cout << "Result Matrix:\n";
    printMat(ansSimple);
    cout << "Execution Time: " << timeSimple << " ms\n\n" << endl;

    int blockSize = 2; //subject to change based on requirements
    auto startBlocking = chrono::high_resolution_clock::now();
    blockingMatMul(mat1, mat2, ansBlocking, blockSize);
    auto endBlocking = chrono::high_resolution_clock::now();
    auto timeBlocking = chrono::duration_cast<chrono::milliseconds>(endBlocking - startBlocking).count();

    cout << "Algorithm: GEMM Blocking\n";
    cout << "Result matrix:\n";
    printMat(ansBlocking);
    cout << "Execution Time: " << timeBlocking << " ms\n\n" << endl;

    return 0;

}
