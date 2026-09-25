#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

struct BIT {
    int n;
    std::vector<int64_t> t;
    BIT(int _n) : n(_n), t(n+1,0) {}
    void update(int i, int64_t v) {
        for (i+=1; i <= n; i += i & -i)
            t[i] = std::max(t[i], v);
    }
    int64_t query(int i) {
        int64_t r = 0;
        for (i += 1; i > 0; i -= i & -i)
           r = std::max(r, t[i]);
        return r;
    }
};

long long maxAlternatingSum(std::vector<int>& nums, int k) {
    int n = nums.size();
    std::vector<int> vals = nums;
    std::sort(vals.begin(), vals.end());
    vals.erase(std::unique(vals.begin(), vals.end()), vals.end());
    auto rankOf = [&](int v) {
        return (int)(std::lower_bound(vals.begin(), vals.end(), v) - vals.begin());
    };
    int m= vals.size();
    BIT bitLow(m), bitHigh(m);
    std::vector<int64_t> low(n), high(n);
    int64_t ans = 0;
    for (int j =0; j < n; ++j) {
        if (j >= k) {
            int i = j-k;
            int ci = rankOf(nums[i]);
            bitLow.update(ci, low[i]);
            bitHigh.update(m-1-ci, high[i]);
        }
        int cj = rankOf(nums[j]);
        high[j] = nums[j] + (cj > 0 ? bitLow.query(cj-1) : 0LL);
        int rev = m-cj-1;
        low[j] = nums[j] + (rev > 0 ? bitHigh.query(rev-1) : 0LL);
        ans = std::max({ans, low[j], high[j]});
    }
    return ans;
}