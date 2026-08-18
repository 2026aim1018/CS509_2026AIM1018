#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <climits>
#include "GEMM/gemm.h"
#include "CSR/csr.h"
#include "SSSP/BellmanFord.h"
#include "SSSP/FloydWarshall.h"

using namespace std;

void printMat(const vector<vector<int>> &mat){
    for(int i=0; i<mat.size(); i++){
        for(int j=0; j<mat[0].size(); j++){
            cout << mat[i][j] << " ";
        }
        cout << " \n";
    }
}

bool readAndConvertToCSR(string fileName, int &V, int &E, int &isWeighted, vector<int> &rowPtr, vector<int> &colIdx, vector<int> &weights){
    ifstream inputFile(fileName);

    if(!inputFile.is_open()){
        cout << "Error: Could not open file " << fileName << endl;
        return false;
    }

    if(!(inputFile >> V >> E >> isWeighted)){
        cout << "Error: Invalid or empty file format." << endl;
        return false;
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
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Error: Missing arguments.\n";
        cout << "Usage: ./DRIVERPROGRAM <input_file> <algorithm>\n";
        cout << "Available algorithms: simple, blocking, csr, bellman-ford, floyd-warshall\n";
        return 1; 
    }
    
    string fileName = argv[1];
    string algorithm = argv[2]; 

    // GEMM: SIMPLE
    if (algorithm == "simple") {
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
    
    //GEMM: BLOCKING
    else if (algorithm == "blocking" or algorithm == "tiling") {
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
    
    //CSR CONVERSION
    else if (algorithm == "csr") {
        int V, E, isWeighted;
        vector<int> rowPtr, colIdx, weights;
        
        auto startCSR = chrono::high_resolution_clock::now();
        
        if (!readAndConvertToCSR(fileName, V, E, isWeighted, rowPtr, colIdx, weights)) {
            return 1;
        }
        
        auto endCSR = chrono::high_resolution_clock::now();
        auto timeCSR = chrono::duration_cast<chrono::milliseconds>(endCSR - startCSR).count();

        cout << "Success: Graph successfully loaded and compressed into CSR format!\n";
        cout << "Vertices: " << V << " | Edges: " << E << "\n";
        cout << "Conversion Time: " << timeCSR << " milliseconds\n";
    }
    
    //GRAPH: BELLMAN-FORD
    else if (algorithm == "bellman-ford") {
        int V, E, isWeighted;
        vector<int> rowPtr, colIdx, weights;
        
        if (!readAndConvertToCSR(fileName, V, E, isWeighted, rowPtr, colIdx, weights)) {
            return 1;
        }
        
        int source = 0; 
        vector<int> distances;
        bool hasNegativeCycle = false;
        
        auto startBF = chrono::high_resolution_clock::now();
        funcBellmanFord(V, rowPtr, colIdx, source, distances, weights, hasNegativeCycle);
        auto endBF = chrono::high_resolution_clock::now();
        auto timeBF = chrono::duration_cast<chrono::milliseconds>(endBF - startBF).count();
        
        cout << "Algorithm: Bellman-Ford\n";
        cout << "Source: " << source << "\n";
        
        if (hasNegativeCycle) {
            cout << "Negative cycle: true\n";
        } else {
            cout << "Vertex Distance\n";
            for (int i = 0; i < V; i++) {
                if (distances[i] == INT_MAX) {
                    cout << i << " INF\n";
                } else {
                    cout << i << " " << distances[i] << "\n";
                }
            }
            cout << "Negative cycle: none\n";
        }
        cout << "Execution time: " << timeBF << " ms\n";
    }

    //GRAPH: FLOYD-WARSHALL
    else if (algorithm == "floyd-warshall") {
        ifstream inputFile(fileName);
        if (!inputFile.is_open()) {
            cout << "Error: Could not open file " << fileName << endl;
            return 1;
        }

        int V;
        inputFile >> V;
        
        vector<vector<int>> dist(V, vector<int>(V));
        string val;

        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                inputFile >> val;
                if (val == "INF") {
                    dist[i][j] = INT_MAX;
                } else {
                    dist[i][j] = stoi(val);
                }
            }
        }
        inputFile.close();

        bool hasNegativeCycle = false;

        auto startFW = chrono::high_resolution_clock::now();
        funcFloydWarshall(V, dist, hasNegativeCycle);
        auto endFW = chrono::high_resolution_clock::now();
        auto timeFW = chrono::duration_cast<chrono::milliseconds>(endFW - startFW).count();
        
        cout << "Algorithm: Floyd-Warshall\n";
        
        if (hasNegativeCycle) {
            cout << "Negative cycle: true\n";
        } else {
            cout << "Distance matrix:\n";
            for (int i = 0; i < V; i++) {
                for (int j = 0; j < V; j++) {
                    if (dist[i][j] == INT_MAX) {
                        cout << "INF ";
                    } else {
                        cout << dist[i][j] << " ";
                    }
                }
                cout << "\n";
            }
            cout << "Negative cycle: none\n";
        }
        cout << "Execution time: " << timeFW << " ms\n";
    }

    else {
        cout << "Error: Unknown algorithm '" << algorithm << "'.\n";
        cout << "Please use 'simple', 'blocking', 'csr', 'bellman-ford', or 'floyd-warshall'.\n";
        return 1;
    }

    return 0;
}