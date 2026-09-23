#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point *data;
    int size;
} PointArray;

int cmpPoint(const void *a, const void *b) {
    const Point *p1 = (const Point *)a;
    const Point *p2 = (const Point *)b;

    if (p1->x < p2->x) return -1;
    if (p1->x > p2->x) return 1;
    return 0;
}

PointArray quickStair(Point *P, int l, int r) {
    if (l == r) {
        PointArray ans;
        ans.data = malloc(sizeof(Point));
        ans.data[0] = P[l];
        ans.size = 1;
        return ans;
    }
    int mid = l + (r - l) / 2;
    PointArray L = quickStair(P, l, mid);
    PointArray R = quickStair(P, mid + 1, r);
    while (L.size > 0 &&
           L.data[L.size - 1].y <= R.data[0].y) {
        L.size--;
    }

    PointArray result;
    result.size = L.size + R.size;
    result.data = malloc(result.size * sizeof(Point));
    int k = 0;
    for (int i = 0; i < L.size; ++i) {
        result.data[k++] = L.data[i];
    }
    for (int i = 0; i < R.size; ++i) {
        result.data[k++] = R.data[i];
    }
    free(L.data);
    free(R.data);
    return result;
}

PointArray staircase(int *X, int *Y, int n) {
    if (n == 0) {
        PointArray empty = {NULL, 0};
        return empty;
    }
    Point *P = malloc(n * sizeof(Point));
    for (int i = 0; i < n; ++i) {
        P[i].x = X[i];
        P[i].y = Y[i];
    }
    qsort(P, n, sizeof(Point), cmpPoint);
    PointArray result = quickStair(P, 0, n - 1);
    free(P);
    return result;
}

void testPareto() {
    int X[] = {1, 2, 3, 4, 5, 6, 7};
    int Y[] = {10, 8, 6, 3, 7, 5, 2};

    int n = sizeof(X) / sizeof(X[0]);

    PointArray pareto = staircase(X, Y, n);
    printf("Pareto-optimal points:\n");
    for (int i = 0; i < pareto.size; ++i) {
        printf("(%d, %d)\n",
               pareto.data[i].x,
               pareto.data[i].y);
    }
    free(pareto.data);

    return 0;
}