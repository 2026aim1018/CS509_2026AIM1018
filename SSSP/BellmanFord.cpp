#include "BellmanFord.h"
#include <vector>
#include <climits>
using namespace std;

void funcBellmanFord(int V, vector<int> &rowPtr, vector<int> &colIdx, int sourceNode, vector<int> &dis, vector<int> &weights, bool &hasNegCycle){
    dis.assign(V, INT_MAX);
    dis[sourceNode] = 0;
    hasNegCycle = false;

    for(int i=0; i<V-1; i++){
        for(int u=0; u<V; u++){
            if(dis[u]==INT_MAX) continue;

            int start = rowPtr[u];
            int end = rowPtr[u+1];

            for(int j=start; j<end; j++){
                int v = colIdx[j];
                int weight = weights[j];
                
                if(dis[u] + weight < dis[v]){
                    dis[v] = dis[u] + weight;
                }
            }
        }
    }
    for(int u =0; u<V; u++){
        if(dis[u]==INT_MAX) continue;

        int start = rowPtr[u];
        int end = rowPtr[u+1];

        for(int j=start; j<end; j++){
            int v = colIdx[j];
            int weight = weights[j];

            if(dis[u] + weight < dis[v]){
                hasNegCycle = true;
                return;
            }
        }
     }
}