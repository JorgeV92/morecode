#include <iostream>
#include <vector>

long long getSum(std::vector<int>& nums) {
    int n = nums.size();
    std::vector<int64_t> P(n+1,0);
    for (int i = 0; i < n; ++i) P[i+1] = P[i] + nums[i];
    std::vector<int> d1(n);
    int l = 0,  r = -1;
    for (int i = 0; i < n; ++i) {
        int k = (i > r) ? 1 : std::min(d1[l+r-i], r-i+1);
        while (i-k >= 0 && i+k < n && nums[i-k] == nums[i+k]) k++;
        d1[i] = k;
        if (i + k -1 > r) { l= i - k + 1; r = i + k -1; }
    }
    std::vector<int> d2(n);
    l = 0, r = -1;
    for (int i = 0; i < n; ++i) {
        int k = (i > r) ? 0 : std::min(d2[l + r - i + 1], r - i + 1);
        while (i - k - 1 >= 0 && i + k < n && nums[i - k - 1] == nums[i + k]) k++;
        d2[i] = k;
        if (i + k - 1 > r) { l = i - k; r = i + k - 1; }
    }
    int64_t ans = 0;
    for (int i = 0; i < n; ++i) {
        if (d1[i] > 0)
            ans = std::max(ans, P[i + d1[i]] - P[i - d1[i] + 1]);
        if (d2[i] > 0)
            ans = std::max(ans, P[i + d2[i]] - P[i - d2[i]]);
    }
    return ans;
}