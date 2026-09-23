#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

int maxDigitRange(std::vector<int>& nums) {
    int n = nums.size();
    std::vector<int> A(n);
    for (int i = 0; i < n; ++i) {
        std::string s = std::to_string(nums[i]);
        int mx = -1e9, mn = 1e9;
        for (char c : s) {
            int x = c - '0';
            mx = std::max(mx, x);
            mn = std::min(mn, x);
        }
        A[i] = mx-mn;
    }
    int h = *std::max_element(A.begin(), A.end());
    int ans = 0;
    for (int i = 0; i < n; i++) {
        if (A[i] == h) ans += nums[i];
    }
    return ans;
}