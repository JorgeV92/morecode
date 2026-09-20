#include <stdlib.h>

static int cmp(const void* a, const void* b) {
    int* const* x = (int* const*)a;
    int* const* y = (int* const*)b; 
    if ((*x)[0] != (*y)[0]) 
        return ((*x)[0] > (*y)[0]) - ((*x)[0] < (*y)[0]);
    return ((*x)[1] > (*y)[1]) - ((*x)[1] < (*y)[1]);
}

static void heap_push(int* heap, int* size, int x) {
    int i = (*size)++;
    heap[i] = x;
    while (i > 0) {
        int p = (i-1) / 2;
        if (heap[p] <= heap[i]) break;
        int tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}

static void heap_pop(int* heap, int* size) {
    heap[0] = heap[--(*size)];
    int i = 0;
    while (1) {
        int l = 2*i+1;
        int r = 2*i+2;
        int sm = i;
        if (l < *size && heap[l] < heap[sm]) sm = l;
        if (r < *size && heap[r] < heap[sm]) sm = r;
        if (sm == i) break;
        int tmp = heap[i];
        heap[i] = heap[sm];
        heap[sm] = tmp;
        i = sm;
    } 
}

long long countIntersectingIntervals(int** intervals, int intervalsSize, int* intervalsColSize) {
    qsort(intervals, intervalsSize, sizeof(int*), cmp);
    int* heap = malloc(intervalsSize * sizeof(int));
    int heap_size = 0;
    long long ans = 0;
    for (int i = 0; i < intervalsSize; ++i) {
        int s= intervals[i][0];
        int e = intervals[i][1];
        while (heap_size > 0 && heap[0] < s) {
            heap_pop(heap, &heap_size);
        }
        ans += heap_size;
        heap_push(heap, &heap_size, e);
    }
    free(heap);
    return ans;
}