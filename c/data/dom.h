#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int size;
    int capacity;
} IntVec;

void vec_init(IntVec *v) {
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

void vec_push(IntVec *v, int x) {
    if (v->size == v->capacity) {
        v->capacity = (v->capacity == 0) ? 4 : v->capacity * 2;

        v->data = realloc(v->data, v->capacity * sizeof(int));
    }
    v->data[v->size++] = x;
}

void vec_clear(IntVec *v) {
    v->size = 0;
}

void vec_free(IntVec *v) {
    free(v->data);

    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}


typedef struct {
    int n;
    int timer;
    IntVec *g;
    IntVec *rg;
    IntVec *bucket;
    int *arr;
    int *rev;
    int *parent;
    int *sdom;
    int *idom;
    int *ancestor;
    int *best;
} LengauerTarjan;

void lt_init(LengauerTarjan *lt, int n) {
    lt->n = n;
    lt->timer = 0;

    lt->g = malloc((n + 1) * sizeof(IntVec));
    lt->rg = malloc((n + 1) * sizeof(IntVec));
    lt->bucket = malloc((n + 1) * sizeof(IntVec));

    for (int i = 0; i <= n; ++i) {
        vec_init(&lt->g[i]);
        vec_init(&lt->rg[i]);
        vec_init(&lt->bucket[i]);
    }

    lt->arr = calloc(n + 1, sizeof(int));
    lt->rev = calloc(n + 1, sizeof(int));

    lt->parent = calloc(n + 1, sizeof(int));

    lt->sdom = calloc(n + 1, sizeof(int));
    lt->idom = calloc(n + 1, sizeof(int));

    lt->ancestor = calloc(n + 1, sizeof(int));
    lt->best = calloc(n + 1, sizeof(int));
}

void lt_add_edge(LengauerTarjan *lt, int u, int v) {
    vec_push(&lt->g[u], v);
}

void lt_dfs(LengauerTarjan *lt, int u) {
    lt->timer++;
    int du = lt->timer;
    lt->arr[u] = du;
    lt->rev[du] = u;
    lt->sdom[du] = du;
    lt->best[du] = du;

    for (int k = 0; k < lt->g[u].size; ++k) {
        int v = lt->g[u].data[k];
        if (lt->arr[v] == 0) {
            lt_dfs(lt, v);
            lt->parent[lt->arr[v]] = lt->arr[u];
        }
        if (lt->arr[v] != 0) {
            vec_push(
                &lt->rg[lt->arr[v]],
                lt->arr[u]
            );
        }
    }
}

void lt_compress(LengauerTarjan *lt, int v) {
    int a = lt->ancestor[v];

    if (a != 0 && lt->ancestor[a] != 0) {

        lt_compress(lt, a);

        int ancestor_best =
            lt->best[lt->ancestor[v]];

        int current_best =
            lt->best[v];

        if (lt->sdom[ancestor_best]
            < lt->sdom[current_best]) {

            lt->best[v] = ancestor_best;
        }

        lt->ancestor[v] =
            lt->ancestor[lt->ancestor[v]];
    }
}

int lt_eval(LengauerTarjan *lt, int v) {
    if (lt->ancestor[v] == 0) {
        return lt->best[v];
    }

    lt_compress(lt, v);

    int ancestor_best =
        lt->best[lt->ancestor[v]];

    int current_best =
        lt->best[v];

    if (lt->sdom[ancestor_best]
        < lt->sdom[current_best]) {

        return ancestor_best;
    }

    return current_best;
}

void lt_link(LengauerTarjan *lt, int parent, int child) {
    lt->ancestor[child] = parent;
}

int *lt_compute(LengauerTarjan *lt, int start) {
    lt_dfs(lt, start);
    int N = lt->timer;
    for (int i = N; i >= 2; --i) {
        for (int k = 0; k < lt->rg[i].size; ++k) {
            int p = lt->rg[i].data[k];
            int u = lt_eval(lt, p);
            if (lt->sdom[u] < lt->sdom[i]) {
                lt->sdom[i] = lt->sdom[u];
            }
        }
        vec_push(&lt->bucket[lt->sdom[i]], i);
        lt_link(lt, lt->parent[i], i);
        int p = lt->parent[i];
        for (int k = 0; k < lt->bucket[p].size; ++k) {
            int v = lt->bucket[p].data[k];
            int u = lt_eval(lt, v);
            if (lt->sdom[u] == lt->sdom[v]) {
                lt->idom[v] = lt->sdom[v];
            } else {
                lt->idom[v] = u;
            }
        }
        vec_clear(&lt->bucket[p]);
    }
    for (int i = 2; i <= N; ++i) {
        if (lt->idom[i] != lt->sdom[i]) {
            lt->idom[i] =
                lt->idom[lt->idom[i]];
        }
    }

    int *result = malloc((lt->n + 1) * sizeof(int));

    for (int i = 0; i <= lt->n; ++i) {
        result[i] = -1;
    }

    result[start] = -1;

    for (int i = 2; i <= N; ++i) {
        int v = lt->rev[i];
        int idom_number = lt->idom[i];
        int idom_vertex = lt->rev[idom_number];
        result[v] = idom_vertex;
    }
    return result;
}

void lt_free(LengauerTarjan *lt) {

    for (int i = 0; i <= lt->n; ++i) {
        vec_free(&lt->g[i]);
        vec_free(&lt->rg[i]);
        vec_free(&lt->bucket[i]);
    }

    free(lt->g);
    free(lt->rg);
    free(lt->bucket);

    free(lt->arr);
    free(lt->rev);

    free(lt->parent);

    free(lt->sdom);
    free(lt->idom);

    free(lt->ancestor);
    free(lt->best);
}