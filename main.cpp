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

// Assignment 3 Headers 
#include "MST/Prims.h"
#include "MST/Kruskals.h"

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


bool readAdjListAndConvertToCSR(string fileName, int &V, int &E, vector<int> &rowPtr, vector<int> &colIdx, vector<int> &weights) {
    ifstream inputFile(fileName);
    if(!inputFile.is_open()){
        cout << "Error: Could not open file " << fileName << endl;
        return false;
    }

    inputFile >> V >> E;
    
    rowPtr.assign(V + 1, 0);
    colIdx.clear();
    weights.clear();

    for(int i = 0; i < V; i++) {
        int u, degree;
        inputFile >> u >> degree;
        
        rowPtr[u] = colIdx.size();
        for(int j = 0; j < degree; j++) {
            int neighbor, weight;
            inputFile >> neighbor >> weight;
            colIdx.push_back(neighbor);
            weights.push_back(weight);
        }
    }
    rowPtr[V] = colIdx.size();
    
    inputFile.close();
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Error: Missing arguments.\n";
        cout << "Usage: ./DRIVERPROGRAM <input_file> <algorithm>\n";
        cout << "Algorithms: simple, blocking, csr, bellman-ford, floyd-warshall, prim, kruskal\n";
        return 1; 
    }
    
    string fileName = argv[1];
    string algorithm = argv[2]; 

    if (algorithm == "simple") {
        ifstream inputFile(fileName);
        if(!inputFile.is_open()){ return 1; }
        int M, K, N; inputFile >> M >> K >> N;
        vector<vector<int>> mat1(M, vector<int>(K, 0)), mat2(K, vector<int>(N, 0)), ansSimple(M, vector<int>(N, 0));
        for(int i=0; i<M; i++) for(int j=0; j<K; j++) inputFile >> mat1[i][j];
        for(int i=0; i<K; i++) for(int j=0; j<N; j++) inputFile >> mat2[i][j];
        inputFile.close();

        auto startSimple = chrono::high_resolution_clock::now();
        simpleMatMul(mat1, mat2, ansSimple);
        auto endSimple = chrono::high_resolution_clock::now();
        cout << "Algorithm: GEMM Simple\nResult matrix:\n"; printMat(ansSimple);
        cout << "Execution time: " << chrono::duration_cast<chrono::milliseconds>(endSimple - startSimple).count() << " ms\n";
    }
    else if (algorithm == "blocking" or algorithm == "tiling") {
        ifstream inputFile(fileName);
        if(!inputFile.is_open()){ return 1; }
        int M, K, N; inputFile >> M >> K >> N;
        vector<vector<int>> mat1(M, vector<int>(K, 0)), mat2(K, vector<int>(N, 0)), ansBlocking(M, vector<int>(N, 0));
        for(int i=0; i<M; i++) for(int j=0; j<K; j++) inputFile >> mat1[i][j];
        for(int i=0; i<K; i++) for(int j=0; j<N; j++) inputFile >> mat2[i][j];
        inputFile.close();

        auto startBlocking = chrono::high_resolution_clock::now();
        blockingMatMul(mat1, mat2, ansBlocking, 32);
        auto endBlocking = chrono::high_resolution_clock::now();
        cout << "Algorithm: GEMM Blocking\nResult matrix:\n"; printMat(ansBlocking);
        cout << "Execution time: " << chrono::duration_cast<chrono::milliseconds>(endBlocking - startBlocking).count() << " ms\n";
    }
    else if (algorithm == "csr") {
        int V, E, isWeighted; vector<int> rowPtr, colIdx, weights;
        auto startCSR = chrono::high_resolution_clock::now();
        if (!readAndConvertToCSR(fileName, V, E, isWeighted, rowPtr, colIdx, weights)) return 1;
        auto endCSR = chrono::high_resolution_clock::now();
        cout << "Success: Graph compressed into CSR!\nVertices: " << V << " | Edges: " << E << "\n";
        cout << "Conversion Time: " << chrono::duration_cast<chrono::milliseconds>(endCSR - startCSR).count() << " ms\n";
    }
    else if (algorithm == "bellman-ford") {
        int V, E, isWeighted; vector<int> rowPtr, colIdx, weights;
        if (!readAndConvertToCSR(fileName, V, E, isWeighted, rowPtr, colIdx, weights)) return 1;
        int source = 0; vector<int> distances; bool hasNegativeCycle = false;
        
        auto startBF = chrono::high_resolution_clock::now();
        funcBellmanFord(V, rowPtr, colIdx, source, distances, weights, hasNegativeCycle);
        auto endBF = chrono::high_resolution_clock::now();
        
        cout << "Algorithm: Bellman-Ford\nSource: " << source << "\n";
        if (hasNegativeCycle) cout << "Negative cycle: true\n";
        else {
            cout << "Vertex Distance\n";
            for (int i = 0; i < V; i++) cout << i << " " << (distances[i] == INT_MAX ? "INF" : to_string(distances[i])) << "\n";
            cout << "Negative cycle: none\n";
        }
        cout << "Execution time: " << chrono::duration_cast<chrono::milliseconds>(endBF - startBF).count() << " ms\n";
    }
    else if (algorithm == "floyd-warshall") {
        ifstream inputFile(fileName);
        if (!inputFile.is_open()) return 1;
        int V; inputFile >> V; vector<vector<int>> dist(V, vector<int>(V)); string val;
        for (int i = 0; i < V; i++) for (int j = 0; j < V; j++) {
            inputFile >> val; dist[i][j] = (val == "INF") ? INT_MAX : stoi(val);
        }
        inputFile.close();
        bool hasNegativeCycle = false;
        
        auto startFW = chrono::high_resolution_clock::now();
        funcFloydWarshall(V, dist, hasNegativeCycle);
        auto endFW = chrono::high_resolution_clock::now();
        
        cout << "Algorithm: Floyd-Warshall\n";
        if (hasNegativeCycle) cout << "Negative cycle: true\n";
        else {
            cout << "Distance matrix:\n";
            for (int i = 0; i < V; i++) {
                for (int j = 0; j < V; j++) cout << (dist[i][j] == INT_MAX ? "INF" : to_string(dist[i][j])) << " ";
                cout << "\n";
            }
            cout << "Negative cycle: none\n";
        }
        cout << "Execution time: " << chrono::duration_cast<chrono::milliseconds>(endFW - startFW).count() << " ms\n";
    }

    else if (algorithm == "prim" || algorithm == "kruskal") {
        int V, E;
        vector<int> rowPtr, colIdx, weights;
        
        if (!readAdjListAndConvertToCSR(fileName, V, E, rowPtr, colIdx, weights)) {
            return 1;
        }

        vector<MSTEdge> mstEdges;
        long long totalWeight = 0;
        long long execTime = 0;

        if (algorithm == "prim") {
            auto start = chrono::high_resolution_clock::now();
            totalWeight = primsAlgoFunc(V, rowPtr, colIdx, weights, mstEdges);
            auto end = chrono::high_resolution_clock::now();
            execTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            cout << "Algorithm: Prim's MST\n";
        } else {
            auto start = chrono::high_resolution_clock::now();
            totalWeight = kruskalsAlgoFunc(V, rowPtr, colIdx, weights, mstEdges);
            auto end = chrono::high_resolution_clock::now();
            execTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            cout << "Algorithm: Kruskal's MST\n";
        }

        cout << "MST edges:\n";
        for (const auto& edge : mstEdges) {
            cout << edge.sourceNode << " " << edge.destNode << " " << edge.weight << "\n";
        }
        cout << "Total MST weight: " << totalWeight << "\n";
        cout << "Execution time: " << execTime << " ms\n";
    }
    
    else {
        cout << "Error: Unknown algorithm '" << algorithm << "'.\n";
        cout << "Available: simple, blocking, csr, bellman-ford, floyd-warshall, prim, kruskal\n";
        return 1;
    }

    return 0;
}