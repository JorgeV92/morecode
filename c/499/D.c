#include <stdlib.h>
#include <string.h>

typedef long long ll;

typedef struct {
    int n;
    ll *t;
} BIT;

void bit_init(BIT *b, int n) {
    b->n = n;
    b->t = (ll *)calloc(n + 1, sizeof(ll));
}
void bit_update(BIT *b, int i, ll v) {         
    for (i += 1; i <= b->n; i += i & -i)
        if (v > b->t[i]) b->t[i] = v;
}
ll bit_query(const BIT *b, int i) {            
    ll r = 0;
    for (i += 1; i > 0; i -= i & -i)
        if (b->t[i] > r) r = b->t[i];
    return r;
}
void bit_free(BIT *b) { free(b->t); }

int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

int rankOf(const int *vals, int m, int v) {
    int lo = 0, hi = m - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (vals[mid] < v) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

long long maxAlternatingSum(int *nums, int numsSize, int k) {
    int n = numsSize;
    int *vals = (int *)malloc(n * sizeof(int));
    memcpy(vals, nums, n * sizeof(int));
    qsort(vals, n, sizeof(int), cmp_int);
    int m = 0;
    for (int i = 0; i < n; i++)
        if (i == 0 || vals[i] != vals[i - 1]) vals[m++] = vals[i];

    BIT bitLow, bitHigh;
    bit_init(&bitLow, m);
    bit_init(&bitHigh, m);                       

    ll *low = (ll *)calloc(n, sizeof(ll));
    ll *high = (ll *)calloc(n, sizeof(ll));
    ll ans = 0;

    for (int j = 0; j < n; j++) {
        if (j >= k) {                            
            int i = j - k;
            int ci = rankOf(vals, m, nums[i]);
            bit_update(&bitLow, ci, low[i]);
            bit_update(&bitHigh, m - 1 - ci, high[i]);
        }
        int cj = rankOf(vals, m, nums[j]);
        high[j] = nums[j] + (cj > 0 ? bit_query(&bitLow, cj - 1) : 0);
        int rev = m - 1 - cj;
        low[j] = nums[j] + (rev > 0 ? bit_query(&bitHigh, rev - 1) : 0);
        if (low[j] > ans) ans = low[j];
        if (high[j] > ans) ans = high[j];
    }
    free(vals); free(low); free(high);
    bit_free(&bitLow); bit_free(&bitHigh);
    return ans;
}

// O(n log n) time, O(n) space