#include "Kruskals.h"
#include <vector>
#include <algorithm>

using namespace std;

struct DisjointSet {
    vector<int> parent;
    vector<int> rank;
    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int findRoot(int i) {
        if (parent[i] == i) {
            return i;
        }
        return parent[i] = findRoot(parent[i]);
    }

    bool uniteNodes(int i, int j) {
        int rootI = findRoot(i);
        int rootJ = findRoot(j);

        if (rootI != rootJ) {
            if (rank[rootI] < rank[rootJ]) {
                parent[rootI] = rootJ;
            } else if (rank[rootI] > rank[rootJ]) {
                parent[rootJ] = rootI;
            } else {
                parent[rootJ] = rootI;
                rank[rootI]++;
            }
            return true;
        }
        return false;
    }
};

bool compareEdges(const MSTEdge& a, const MSTEdge& b) {
    return a.weight < b.weight;
}

long long kruskalsAlgoFunc(int V, const vector<int> &rowPtr, const vector<int> &colIdx, const vector<int> &weights, vector<MSTEdge> &mstEdges){
    long long totalTreeWeight = 0;
    vector<MSTEdge> allEdges;

    for(int u=0; u<V; u++){
        for(int i=rowPtr[u]; i<rowPtr[u+1]; i++){
            int v = colIdx[i];
            int weight = weights[i];

            if(u<v){
                allEdges.push_back({u, v, weight});
            }
        }
    }

    sort(allEdges.begin(), allEdges.end(), compareEdges);

    DisjointSet dsu(V);

    for(const auto&edge: allEdges){
        if(dsu.uniteNodes(edge.sourceNode, edge.destNode)){
            mstEdges.push_back(edge);
            totalTreeWeight += edge.weight;

            if(mstEdges.size()==V-1){
                break;
            }
        }
    }
    return totalTreeWeight;
}