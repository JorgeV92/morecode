#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAXN 100000

typedef struct {
    int l, r, idx;
} Queery; 

static int block_size;

int comp(const void* a, const void* b) {
    const Queery* qa = (const Queery*)a;
    const Queery* qb = (const Queery*)b;
    int blockA = qa->l / block_size;
    int blockB = qb->l / block_size;
    if (blockA != blockB) return blockA - blockB;
    return qa->r - qb->r;
}

void add(int idx, int* nums, int* freq, int *d, int *odd) {
    int x = nums[idx];
    if (freq[x] == 0) (*d)++;
    if (freq[x] % 2 == 0) (*odd)++;
    else (*odd)--;
    freq[x]++;
}

void _remove(int idx, int* nums, int* freq, int* d, int* odd) {
    int x = nums[idx];
    if (freq[x] % 2 == 0) (*odd)++;
    else (*odd)--;
    freq[x]--;
    if (freq[x] == 0) (*d)--;
}

bool* validSubarrays(int* nums, int numsSize, int k, int** queries, 
                    int queriesSize, int* queriesColSize, int* returnSize) {
    int n = numsSize;
    int q = queriesSize;
    block_size = (int)sqrt(n) + 1;
    Queery* qs = malloc(q * sizeof(Queery));
    bool *ans = malloc(q * sizeof(bool));
    if (qs == NULL || ans == NULL) {
        free(qs); free(ans); 
        *returnSize = 0;
        return NULL;
    }
    for (int i = 0; i < q; i++) {
        qs[i].l = queries[i][0];
        qs[i].r = queries[i][1];
        qs[i].idx = i;
    }
    qsort(qs, q, sizeof(Queery), comp);
    int *freq = calloc(MAXN+1,sizeof(int));
    if (freq == NULL) { free(qs); free(ans); *returnSize=0; return NULL; }
    int d = 0;
    int odd = 0;
    int cur_l = 0;
    int cur_r = -1;
    for (int i = 0; i < q; i++) {
        Queery qq = qs[i];
        while (cur_l > qq.l) {
            cur_l--;
            add(cur_l, nums, freq, &d, &odd);
        }
        while (cur_r < qq.r) {
            cur_r++;
            add(cur_r, nums, freq, &d, &odd);
        }
        while (cur_l < qq.l) {
            _remove(cur_l, nums, freq, &d, &odd);
            cur_l++;
        }
        while (cur_r > qq.r) {
            _remove(cur_r, nums, freq, &d, &odd);
            cur_r--;
        }
        ans[qq.idx] = (d == k && odd == 0);
    }
    free(qs);
    free(freq);
    *returnSize = q;
    return ans;
}