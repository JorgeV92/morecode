
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
