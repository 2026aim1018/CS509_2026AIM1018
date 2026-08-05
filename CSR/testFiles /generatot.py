import random

# --- PARAMETERS ---
V = 10              # Number of vertices
is_weighted = True     # Change to True if you want a weighted graph
# ---------------------------------

filename = f"csr_test_weighted{V}.txt"
print(f"Generating {filename} with V = {V}...")

edges = []

for i in range(V - 1):
    edges.append((i, i + 1))

extra_edges = V * 3
for _ in range(extra_edges):
    u = random.randint(0, V - 1)
    v = random.randint(0, V - 1)
    if u != v:
        edges.append((u, v))

edges = list(set(edges))
E = len(edges)

with open(filename, 'w') as f:
    weight_flag = 1 if is_weighted else 0
    f.write(f"{V} {E} {weight_flag}\n")
    
    for u, v in edges:
        if is_weighted:
            weight = random.randint(1, 50)
            f.write(f"{u} {v} {weight}\n")
        else:
            f.write(f"{u} {v}\n")
            
print(f"Success! Saved as {filename}")