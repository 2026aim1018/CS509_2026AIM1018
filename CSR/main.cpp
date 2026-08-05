#include "csr.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

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
        cout << "Usage: ./DRIVERPROGRAM <filename>\n";
        return 1;
    }

    string fileName = argv[1];
    cout << "Target File: " << fileName << "\n";
    
    auto startCSR = chrono::high_resolution_clock::now();
    readAndConvertToCSR(fileName);
    auto endCSR = chrono::high_resolution_clock::now();
    auto timeCSR = chrono::duration_cast<chrono::milliseconds>(endCSR - startCSR).count();

    return 0;
}
