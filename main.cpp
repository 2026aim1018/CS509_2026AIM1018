#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include "GEMM/gemm.h"
#include "CSR/csr.h"

using namespace std;

void printMat(const vector<vector<int>> &mat){
    for(int i=0; i<mat.size(); i++){
        for(int j=0; j<mat[0].size(); j++){
            cout << mat[i][j] << " ";
        }
        cout << "\n";
    }
}

void readAndConvertToCSR(string fileName, int &V, int &E, int &isWeighted, vector<int> &rowPtr, vector<int> &colIdx, vector<int> &weights){
    ifstream inputFile(fileName);

    if(!inputFile.is_open()){
        cout << "Error: Could not open file " << fileName << endl;
        return;
    }

    if(!(inputFile >> V >> E >> isWeighted)){
        cout << "Error: Invalid or empty file format." << endl;
        return;
    }

    if(isWeighted == 0){
        vector<vector<int>> adjList(V);
        for(int i = 0; i < E; i++){
            int u, v;
            inputFile >> u >> v;
            adjList[u].push_back(v);
        }
        convertToCSR_Unweighted(adjList, rowPtr, colIdx);
    }
    else{
        vector<vector<pair<int, int>>> adjList(V);
        for(int i = 0; i < E; i++){
            int u, v, weight;
            inputFile >> u >> v >> weight;
            adjList[u].push_back(make_pair(v, weight));
        }
        convertToCSR_Weighted(adjList, rowPtr, colIdx, weights);
    }
    inputFile.close();
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Error: Missing arguments.\n";
        cout << "Usage: ./DRIVERPROGRAM <input_file> <algorithm>\n";
        cout << "Available algorithms: simple, blocking, csr\n";
        return 1; 
    }
    
    string fileName = argv[1];
    string algorithm = argv[2]; 

    if (algorithm == "simple") {
        cout << "Running Simple GEMM\n";
        ifstream inputFile(fileName);
        if(!inputFile.is_open()){
            cout << "Error: Could not open file " << fileName << endl;
            return 1;
        }

        int M, K, N;
        inputFile >> M >> K >> N;

        vector<vector<int>> mat1(M, vector<int>(K, 0));
        vector<vector<int>> mat2(K, vector<int>(N, 0));
        vector<vector<int>> ansSimple(M, vector<int>(N, 0));

        for(int i=0; i<M; i++) {
            for(int j=0; j<K; j++) inputFile >> mat1[i][j];
        }
        for(int i=0; i<K; i++) {
            for(int j=0; j<N; j++) inputFile >> mat2[i][j];
        }
        inputFile.close();

        auto startSimple = chrono::high_resolution_clock::now();
        simpleMatMul(mat1, mat2, ansSimple);
        auto endSimple = chrono::high_resolution_clock::now();
        auto timeSimple = chrono::duration_cast<chrono::milliseconds>(endSimple - startSimple).count();

        cout << "Algorithm: GEMM Simple\n";
        cout << "Result matrix:\n"; 
        printMat(ansSimple);
        cout << "Execution time: " << timeSimple << " ms\n";
    }
    
    else if (algorithm == "blocking" or algorithm == "tiling") {
        cout << "Running Blocking GEMM\n";
        ifstream inputFile(fileName);
        if(!inputFile.is_open()){
            cout << "Error: Could not open file " << fileName << endl;
            return 1;
        }

        int M, K, N;
        inputFile >> M >> K >> N;

        vector<vector<int>> mat1(M, vector<int>(K, 0));
        vector<vector<int>> mat2(K, vector<int>(N, 0));
        vector<vector<int>> ansBlocking(M, vector<int>(N, 0));

        for(int i=0; i<M; i++) {
            for(int j=0; j<K; j++) inputFile >> mat1[i][j];
        }
        for(int i=0; i<K; i++) {
            for(int j=0; j<N; j++) inputFile >> mat2[i][j];
        }
        inputFile.close();

        int blockSize = 32; 
        auto startBlocking = chrono::high_resolution_clock::now();
        blockingMatMul(mat1, mat2, ansBlocking, blockSize);
        auto endBlocking = chrono::high_resolution_clock::now();
        auto timeBlocking = chrono::duration_cast<chrono::milliseconds>(endBlocking - startBlocking).count();

        cout << "Algorithm: GEMM Blocking\n";
        cout << "Result matrix:\n";
        printMat(ansBlocking);
        cout << "Execution time: " << timeBlocking << " ms\n";
    }
    else if (algorithm == "csr") {
        cout << "--- Running CSR Conversion ---\n";
        int V, E, isWeighted;
        vector<int> rowPtr, colIdx, weights;
        
        auto startCSR = chrono::high_resolution_clock::now();
        
        readAndConvertToCSR(fileName, V, E, isWeighted, rowPtr, colIdx, weights);
        
        auto endCSR = chrono::high_resolution_clock::now();
        
        auto timeCSR = chrono::duration_cast<chrono::microseconds>(endCSR - startCSR).count();

        cout << "Success: Graph successfully loaded and compressed into CSR format!\n";
        cout << "Vertices: " << V << " | Edges: " << E << "\n";
        cout << "Conversion Time: " << timeCSR << " microseconds\n\n";
    }
    
    else {
        cout << "Error: Unknown algorithm '" << algorithm << "'.\n";
        cout << "Please use 'simple', 'blocking', or 'csr'.\n";
        return 1;
    }

    return 0;
}