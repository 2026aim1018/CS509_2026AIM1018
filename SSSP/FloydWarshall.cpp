#include "FloydWarshall.h"
#include <vector>
#include <climits>

using namespace std;

void funcFloydWarshall(int V, vector<vector<int>> &dist, bool &hasNegCycle) {
    hasNegCycle = false;

    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }

    for (int i = 0; i < V; i++) {
        if (dist[i][i] < 0) {
            hasNegCycle = true;
            return;
        }
    }
}