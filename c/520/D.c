#include <stdlib.h>

long long maxValue(int* nums, int numsSize) {
    int n = numsSize;
    long long* P = malloc((n + 1) * sizeof(long long));
    P[0] = 0;
    for (int i = 0; i < n; ++i) {
        if (i % 2 == 0) P[i + 1] = P[i] + nums[i];
        else P[i + 1] = P[i] - nums[i];
    }
    long long base = P[n];
    const long long NEG = -(1LL << 60);
    long long bestP[2] = {NEG, NEG};
    long long bestP1[2] = {NEG, NEG};
    long long bestGain = 0;
    for (int r = 1; r < n; ++r) {
        int l = r - 1;
        int lp = l & 1;
        if (P[l] > bestP[lp])
            bestP[lp] = P[l];
        if (P[l + 1] > bestP1[lp])
            bestP1[lp] = P[l + 1];
        int rp = r & 1;
        if (bestP1[rp] != NEG) {
            long long gain =
                2LL * (bestP1[rp] - P[r + 1]);
            if (gain > bestGain)
                bestGain = gain;
        }
        if (bestP[rp ^ 1] != NEG) {
            long long gain =
                2LL * (bestP[rp ^ 1] - P[r + 1]);

            if (gain > bestGain)
                bestGain = gain;
        }
    }
    long long ans = base + bestGain;
    free(P);
    return ans;
}