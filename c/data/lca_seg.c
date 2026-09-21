#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int n;

    int *height;
    int *first;
    int *euler;
    int euler_size;

    int *segtree;
    bool *visited;
} LCA;


/* ---------- DFS / Euler Tour ---------- */

void lca_dfs(LCA *lca, int node, int h, int *head, int *to, int *next) {
    lca->visited[node] = true;
    lca->height[node] = h;
    /* first occurrence of node in Euler tour */
    lca->first[node] = lca->euler_size;
    lca->euler[lca->euler_size++] = node;
    for (int e = head[node]; e != -1; e = next[e]) {
        int v = to[e];
        if (!lca->visited[v]) {
            lca_dfs(lca, v, h + 1, head, to, next);
            /*
             * After returning from the child,
             * add the current node again.
             */
            lca->euler[lca->euler_size++] = node;
        }
    }
}
/* ---------- Segment Tree ---------- */

void lca_build(LCA *lca, int node, int left, int right) {
    if (left == right) {
        lca->segtree[node] = lca->euler[left];
        return;
    }

    int mid = (left + right) / 2;

    lca_build(lca, node * 2, left, mid);
    lca_build(lca, node * 2 + 1, mid + 1, right);

    int a = lca->segtree[node * 2];
    int b = lca->segtree[node * 2 + 1];

    if (lca->height[a] < lca->height[b])
        lca->segtree[node] = a;
    else
        lca->segtree[node] = b;
}


/* ---------- RMQ query ---------- */

int lca_query(LCA *lca, int node, int left, int right, int ql, int qr) {
    /* no overlap */
    if (right < ql || left > qr)
        return -1;
    /* completely inside */
    if (ql <= left && right <= qr)
        return lca->segtree[node];
    int mid = (left + right) / 2;
    int a = lca_query(
        lca,
        node * 2,
        left,
        mid,
        ql,
        qr
    );

    int b = lca_query(
        lca,
        node * 2 + 1,
        mid + 1,
        right,
        ql,
        qr
    );

    if (a == -1)
        return b;

    if (b == -1)
        return a;

    return lca->height[a] < lca->height[b] ? a : b;
}


/* ---------- LCA ---------- */

int get_lca(LCA *lca, int u, int v) {
    int left = lca->first[u];
    int right = lca->first[v];

    if (left > right) {
        int tmp = left;
        left = right;
        right = tmp;
    }

    return lca_query(
        lca,
        1,
        0,
        lca->euler_size - 1,
        left,
        right
    );
}

void lca_init(LCA *lca, int n, int root, int *head, int *to, int *next) {
    lca->n = n;
    lca->height = malloc(n * sizeof(int));
    lca->first = malloc(n * sizeof(int));
    lca->visited = calloc(n, sizeof(bool));
    /*
     * For a tree with n vertices:
     *
     * Euler tour length = 2*n - 1
     */
    lca->euler = malloc((2 * n) * sizeof(int));
    lca->euler_size = 0;

    lca_dfs(
        lca,
        root,
        0,
        head,
        to,
        next
    );
    /*
     * Segment tree requires about 4*m elements.
     */
    int m = lca->euler_size;

    lca->segtree = malloc((4 * m) * sizeof(int));

    lca_build(lca, 1, 0, m - 1);
}

void lca_free(LCA *lca) {
    free(lca->height);
    free(lca->first);
    free(lca->euler);
    free(lca->segtree);
    free(lca->visited);
}

int main(void) {
    int n = 5;
    int edges = n - 1;

    int *head = malloc(n * sizeof(int));
    int *to = malloc(2 * edges * sizeof(int));
    int *next = malloc(2 * edges * sizeof(int));

    for (int i = 0; i < n; ++i)
        head[i] = -1;

    int edge_count = 0;

    #define ADD_EDGE(u, v)              \
        do {                            \
            to[edge_count] = (v);       \
            next[edge_count] = head[u]; \
            head[u] = edge_count++;     \
        } while (0)

    /* 0 - 1 */
    ADD_EDGE(0, 1);
    ADD_EDGE(1, 0);

    /* 0 - 2 */
    ADD_EDGE(0, 2);
    ADD_EDGE(2, 0);

    /* 1 - 3 */
    ADD_EDGE(1, 3);
    ADD_EDGE(3, 1);

    /* 1 - 4 */
    ADD_EDGE(1, 4);
    ADD_EDGE(4, 1);

    LCA lca;

    lca_init(&lca, n, 0, head, to, next);

    printf("LCA(3, 4) = %d\n", get_lca(&lca, 3, 4));
    printf("LCA(3, 2) = %d\n", get_lca(&lca, 3, 2));
    printf("LCA(1, 4) = %d\n", get_lca(&lca, 1, 4));

    lca_free(&lca);

    free(head);
    free(to);
    free(next);

    return 0;
}