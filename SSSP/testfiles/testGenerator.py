import random
import os

def generate_bellman_ford_test(filename, V, E, has_negative_cycle=False):
    edges = set()
    adj = {i: [] for i in range(V)}
    
    # Try to connect graph first from source 0
    for i in range(1, V):
        edges.add((0, i))
        adj[0].append((i, random.randint(1, 10)))
        
    while len(edges) < E:
        u = random.randint(0, V - 1)
        v = random.randint(0, V - 1)
        if u != v and (u, v) not in edges:
            weight = random.randint(-1, 15) 
            edges.add((u, v))
            adj[u].append((v, weight))

    with open(filename, 'w') as f:
        f.write(f"{V} {len(edges)}\n")
        for u in range(V):
            line = f"{u} {len(adj[u])}"
            for v, w in adj[u]:
                line += f" {v} {w}"
            f.write(line + "\n")
        f.write("SOURCE 0\n")
    print(f"Generated {filename}")

def generate_floyd_warshall_test(filename, V, density=0.4):
    matrix = [["INF" for _ in range(V)] for _ in range(V)]
    for i in range(V):
        matrix[i][i] = 0
        for j in range(V):
            if i != j and random.random() < density:
                matrix[i][j] = random.randint(-1, 15)
                
    with open(filename, 'w') as f:
        f.write(f"{V}\n")
        for row in matrix:
            f.write(" ".join(str(val) for val in row) + "\n")
    print(f"Generated {filename}")

def generate_undirected_unweighted_test(filename, V, E):
    edges = set()
    adj = {i: [] for i in range(V)}
    
    while len(edges) < E:
        u = random.randint(0, V - 1)
        v = random.randint(0, V - 1)
        if u != v and tuple(sorted((u, v))) not in edges:
            edges.add(tuple(sorted((u, v))))
            adj[u].append(v)
            adj[v].append(u)

    with open(filename, 'w') as f:
        f.write(f"{V} {len(edges)}\n")
        for u in range(V):
            line = f"{u} {len(adj[u])}"
            for v in sorted(adj[u]):  # Sorted for Triangle Counting optimization
                line += f" {v}"
            f.write(line + "\n")
    print(f"Generated {filename}")

if __name__ == "__main__":
    # Creates a dedicated folder for all test files
    os.makedirs("testFiles2", exist_ok=True)
    os.chdir("testFiles2")
    
    print("--- Generating Bellman-Ford Tests ---")
    # Sizes: 10, 100, 10,000, 50,000, 100,000. Edges kept sparse (approx 3V)
    bf_sizes = [(10, 30), (100, 300), (10000, 30000), (50000, 150000), (100000, 300000)] 
    for v, e in bf_sizes:
        generate_bellman_ford_test(f"bf_{v}.txt", v, e)
        
    print("\n--- Generating Floyd-Warshall Tests ---")
    fw_sizes = [10, 100, 500, 1000, 2000] 
    for v in fw_sizes:
        generate_floyd_warshall_test(f"fw_{v}.txt", v)
        
    print("\n--- Generating Triangle Counting Tests ---")
    tc_sizes = [(10, 30), (100, 300), (10000, 30000), (50000, 150000), (100000, 300000)]
    for v, e in tc_sizes:
        generate_undirected_unweighted_test(f"tc_{v}.txt", v, e)

    print("\n--- Generating Betweenness Centrality Tests ---")
    bc_sizes = [(10, 30), (100, 300), (1000, 3000), (5000, 15000), (10000, 30000)]
    for v, e in bc_sizes:
        generate_undirected_unweighted_test(f"bc_{v}.txt", v, e)

    print("\n--- Generating Connected Components Tests ---")
    cc_sizes = [(10, 30), (100, 300), (10000, 30000), (50000, 150000), (100000, 300000)]
    for v, e in cc_sizes:
        generate_undirected_unweighted_test(f"cc_{v}.txt", v, e)
        
    print("\nAll test files generated successfully in the 'testFiles2' folder!")