#ifndef DSU2_H
#define DSU2_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int *e;  /* e[i] < 0: i is a root of size -e[i]; otherwise e[i] is the parent */
    int n;
} DSU;

static inline bool dsu_init(DSU *d, int n) {
    d->e = (int *)malloc(n * sizeof(int));
    if (!d->e) { d->n = 0; return false; }
    d->n = n;
    for (int i = 0; i < n; i++) d->e[i] = -1;
    return true;
}

static inline void dsu_free(DSU *d) {
    free(d->e);
    d->e = NULL;
    d->n = 0;
}

static inline int dsu_get(DSU *d, int x) {
    return d->e[x] < 0 ? x : (d->e[x] = dsu_get(d, d->e[x]));
}

static inline bool dsu_same_set(DSU *d, int a, int b) {
    return dsu_get(d, a) == dsu_get(d, b);
}

static inline int dsu_size(DSU *d, int x) {
    return -d->e[dsu_get(d, x)];
}

static inline bool dsu_unite(DSU *d, int x, int y) {
    x = dsu_get(d, x);
    y = dsu_get(d, y);
    if (x == y) return false;
    if (d->e[x] > d->e[y]) { int t = x; x = y; y = t; }
    d->e[x] += d->e[y];
    d->e[y] = x;
    return true;
}

#endif /* DSU_H */