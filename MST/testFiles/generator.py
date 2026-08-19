import random
import os

def generate_mst_graph(filename, V, E):
    print(f"Generating {filename} with {V} vertices and {E} edges...")
    
    edges_set = set()
    adj = {i: [] for i in range(V)}
    
    def add_edge(u, v, w):
        edge_tuple = (min(u, v), max(u, v))
        edges_set.add(edge_tuple)
        
        adj[u].append((v, w))
        adj[v].append((u, w))


    for i in range(1, V):
        u = random.randint(0, i - 1)
        w = random.randint(-20, 100)
        add_edge(u, i, w)
        
    while len(edges_set) < E:
        u = random.randint(0, V - 1)
        v = random.randint(0, V - 1)
        
        if u == v:
            continue 
            
        edge_tuple = (min(u, v), max(u, v))
        if edge_tuple not in edges_set:
            w = random.randint(-20, 100)
            add_edge(u, v, w)
            

    with open(filename, 'w') as f:
  
        f.write(f"{V} {E}\n")
        
        for u in range(V):
          
            line = f"{u} {len(adj[u])}"
            for v, w in adj[u]:
                line += f" {v} {w}"
            f.write(line + "\n")
            
    print(f"Successfully created {filename}")

if __name__ == "__main__":
    os.makedirs("testfiles", exist_ok=True)
    os.chdir("testfiles")
    
    print("--- Generating MST Test Cases ---")

    mst_sizes = [
        (10, 25),
        (100, 300),
        (10000, 30000),
        (50000, 150000),
        (100000, 300000)
    ]
    
    for V, E in mst_sizes:
        generate_mst_graph(f"mst_{V}.txt", V, E)
        
    print("\nAll MST files generated perfectly!")