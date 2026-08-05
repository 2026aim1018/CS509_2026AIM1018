import random

def generate_gemm_file(filename, M, K, N):
    print(f"Generating {filename} with dimensions M={M}, K={K}, N={N}...")
    
    with open(filename, 'w') as f:
        f.write(f"{M} {K} {N}\n")
        
        for _ in range(M):
            row = [str(random.randint(1, 9)) for _ in range(K)]
            f.write(" ".join(row) + "\n")
            
        for _ in range(K):
            row = [str(random.randint(1, 9)) for _ in range(N)]
            f.write(" ".join(row) + "\n")
            
    print("File generation complete!")

M, K, N = 100, 100, 100 
generate_gemm_file("gemm_test_100.txt", M, K, N)