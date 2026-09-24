#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std; 

int divisibleGame(vector<int>& nums) {
    const long long MOD = 1000000007;
    int n = nums.size();

    vector<long long> P(n + 1, 0);             
    for (int i = 0; i < n; i++) P[i + 1] = P[i] + nums[i];

    map<int, vector<int>> bucket;            
    for (int i = 0; i < n; i++) {
        long long v = nums[i];
        for (long long d = 2; d * d <= v; d++)
            if (v % d == 0) {
                bucket[d].push_back(i);
                if (d * d != v) bucket[v / d].push_back(i);
            }
        if (v > 1) bucket[v].push_back(i);
    }

    long long bestVal = LLONG_MIN, bestK = LLONG_MAX;
    for (auto& [k, pos] : bucket) {
        long long cur = nums[pos[0]], best = cur;
        for (int t = 1; t < (int)pos.size(); t++) {
            long long gap = P[pos[t]] - P[pos[t - 1] + 1];  
            cur = max((long long)nums[pos[t]], cur - gap + nums[pos[t]]);
            best = max(best, cur);
        }
        if (best > bestVal) { bestVal = best; bestK = k; }  
    }

    long long emptyVal = -*min_element(nums.begin(), nums.end());
    long long emptyK = 2;
    while (bucket.count(emptyK)) emptyK++;
    if (emptyVal > bestVal || (emptyVal == bestVal && emptyK < bestK)) {
        bestVal = emptyVal; bestK = emptyK;
    }

    long long ans = (bestVal % MOD) * (bestK % MOD) % MOD;
    return (int)((ans + MOD) % MOD);            
}