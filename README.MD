# CS509 - Assignment 1 & 2 (2026AIM1018)
**Task Type:** Single Task (GEMM & CSR) and Buddy Tasks (BFS, DFS, SSSP)

### GEMM Results Table

| Test File | Input Type / Size | Simple Time | Blocking Time | Block Size | Status |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `gemm_test_100.txt` | 100 x 100 | 9 ms | 12 ms | 32 | Pass |
| `gemm_test_500.txt` | 500 x 500 | 734 ms | 1102 ms | 32 | Pass |

### CSR Preprocessing Timing Log 

| Vertices (V) | Graph Type | CSR Conversion Time |
| :--- | :--- | :--- |
| 10 | Unweighted | 0 ms |
| 100 | Unweighted | 0 ms |
| 10,000 | Unweighted | 21 ms |
| 50,000 | Unweighted | 80 ms |
| 100,000 | Unweighted | 152 ms |
| 10 | Weighted | 0 ms |
| 100 | Weighted | 0 ms |
| 10,000 | Weighted | 25 ms |
| 50,000 | Weighted | 104 ms |
| 100,000 | Weighted | 194 ms |

### Bellman-Ford / Floyd-Warshall Results Table

| Algorithm | Test File | Vertices | Edges | Source | Negative Cycle | Expected Output | Actual Output | Time | Status |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Bellman-Ford | `bf_10.txt` | 10 | 30 | 0 | No | Distances | Distances | 0 ms | Pass |
| Floyd-Warshall | `fw_10.txt` | 10 | N/A | N/A | No | Distance matrix | Distance matrix | 0 ms | Pass |
| Bellman-Ford | `bf_100.txt` | 100 | 300 | 0 | No | Distances | Distances | 0 ms | Pass |
| Floyd-Warshall | `fw_100.txt` | 100 | N/A | N/A | No | Distance matrix | Distance matrix | 1 ms | Pass |
| Floyd-Warshall | `fw_500.txt` | 500 | N/A | N/A | No | Distance matrix | Distance matrix | 35 ms | Pass |
| Floyd-Warshall | `fw_1000.txt` | 1000 | N/A | N/A | No | Distance matrix | Distance matrix | 180 ms | Pass |
| Floyd-Warshall | `fw_2000.txt` | 2000 | N/A | N/A | No | Distance matrix | Distance matrix | 1450 ms | Pass |
| Bellman-Ford | `bf_10000.txt` | 10000 | 30000 | 0 | No | Distances | Distances | 85 ms | Pass |
| Bellman-Ford | `bf_50000.txt` | 50000 | 150000 | 0 | No | Distances | Distances | 2140 ms | Pass |
| Bellman-Ford | `bf_100000.txt` | 100000 | 300000 | 0 | No | Distances | Distances | 8500 ms | Pass |

### Minimum Spanning Tree (Prim's vs. Kruskal's) Results Table

| Test File | Total MST Weight | Prim's Time | Kruskal's Time | Status |
| :--- | :--- | :--- | :--- | :--- |
| `mst_10.txt` | 122 | 0 ms | 0 ms | Pass |
| `mst_100.txt` | 624 | 0 ms | 0 ms | Pass |
| `mst_10000.txt` | 32079 | 20 ms | 4 ms | Pass |
| `mst_50000.txt` | 166990 | 92 ms | 19 ms | Pass |
| `mst_100000.txt` | 336013 | 192 ms | 40 ms | Pass |