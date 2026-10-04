#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>          
#include <stdio.h>
#include <stdlib.h>

void uf_init(size_t n, size_t parent[static n]) {
    for (size_t i = 0; i < n; ++i) 
        parent[i] = SIZE_MAX;
}

size_t uf_find(size_t const parent[static 1], size_t i) {
    while (parent[i] != SIZE_MAX) 
        i = parent[i];
    return i;
}

size_t uf_find_replace(size_t parent[static 1], size_t i, size_t x) {
    for (;;) {
        size_t nxt = parent[i];
        parent[i] = x;
        if (nxt == SIZE_MAX) 
            return i;
        i = nxt;
    }
}

size_t uf_find_compress(size_t parent[static 1], size_t i) {
    size_t root = uf_find(parent, i);
    while (parent[i] != SIZE_MAX) {
        size_t nxt = parent[i];
        parent[i] = root;
        i = nxt;
    }
    return root;
}

void uf_union(size_t parent[static 1], size_t i, size_t j) {
    size_t ri = uf_find_compress(parent, i);
    size_t rj = uf_find(parent, j);
    if (ri != rj)
        uf_find_replace(parent, j, ri);
}

void uf_find_by_size(size_t parent[], size_t sz[], size_t i, size_t j) {
    size_t ri = uf_find_compress(parent, i);
    size_t rj = uf_find_compress(parent, j);
    if (ri == rj) return;
    if (sz[ri] < sz[rj]) { size_t t = ri; ri = rj; rj = t; }
    uf_find_replace(parent, rj, ri);
    sz[ri] += sz[rj];
}

bool uf_connected(size_t const parent[static 1], size_t i, size_t j) {
    return uf_find(parent, i) == uf_find(parent, j);
}

void test_uf() {
     enum { N = 10 };
    size_t parent[N];
    uf_init(N, parent);

    uf_union(parent, 0, 1);
    uf_union(parent, 1, 2);
    uf_union(parent, 2, 3);
    uf_union(parent, 4, 5);
    uf_union(parent, 5, 6);
    uf_union(parent, 7, 8);

    uf_union(parent, 0, 3);   
    uf_union(parent, 9, 9);   

    uf_union(parent, 3, 4);

    for (size_t i = 0; i < N; ++i)
        printf("find(%zu) = %zu\n", i, uf_find_compress(parent, i));

    assert( uf_connected(parent, 0, 6));
    assert(!uf_connected(parent, 0, 7));
    assert( uf_connected(parent, 7, 8));
    assert(!uf_connected(parent, 8, 9));

    
    size_t components = 0;
    for (size_t i = 0; i < N; ++i)
        components += (parent[i] == SIZE_MAX);
    printf("%zu components: {0..6}, {7,8}, {9}\n", components);
    assert(components == 3);

    
    enum { M = 1000, OPS = 4000 };
    size_t p[M];
    size_t united[OPS][2];
    uf_init(M, p);
    srand(20261004);
    for (size_t k = 0; k < OPS; ++k) {
        size_t a = (size_t)rand() % M;
        size_t b = (size_t)rand() % M;
        uf_union(p, a, b);
        united[k][0] = a;
        united[k][1] = b;
    }
    
    for (size_t k = 0; k < OPS; ++k)
        assert(uf_connected(p, united[k][0], united[k][1]));
    
    
    for (size_t i = 0; i < M; ++i) {
        size_t x = i, steps = 0;
        while (p[x] != SIZE_MAX) {
            x = p[x];
            assert(++steps <= M);
        }
    }
    puts("all tests passed");
}