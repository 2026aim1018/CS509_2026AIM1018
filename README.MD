# CS509 - Assignment 1 (2026AIM1018)
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

### Graph Results Table
*(To be completed after implementing Buddy Tasks[cite: 2])*

| Algorithm | Test File | Vertices | Edges | Input Type | Source | Time | Status |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| BFS | `bfs_10.txt` | 10 | ... | Unweighted adjacency list | 0 | ... ms | TBD |
| DFS | `dfs_10.txt` | 10 | ... | Unweighted adjacency list | 0 | ... ms | TBD |
| SSSP | `sssp_10.txt` | 10 | ... | Positive weighted adjacency list | 0 | ... ms | TBD |