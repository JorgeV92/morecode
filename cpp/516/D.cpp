#include <iostream>
#include <vector>
#include <algorithm>

std::vector<bool> validSubarrays(std::vector<int>& nums, int k, std::vector<std::vector<int>>& queries) {
    int n = nums.size();
    int q = queries.size();
    int block_size = std::sqrt(n) + 1;
    
    struct Query { 
        int l, r, idx;
    };

    std::vector<Query> qs;
    for (int i = 0; i < q; i++) {
        qs.push_back({queries[i][0], queries[i][1], i});
    }
    std::sort(qs.begin(), qs.end(), 
        [&](const Query& a, const Query& b) {
            int blockA = a.l / block_size;
            int blockB = b.l / block_size;
            if (blockA != blockB) return blockA < blockB;
            return a.r < b.r;
    });
    std::vector<int> freq(100001,0);
    int d = 0;
    int odd = 0;

    auto add = [&](int idx) {
        int x = nums[idx];
        if (freq[x] == 0) d++;
        if (freq[x] % 2 == 0) odd++;
        else odd--;
        freq[x]++;
    };

    auto remove = [&](int idx) {
        int x = nums[idx];
        if (freq[x] % 2 == 0) odd++;
        else odd--;
        freq[x]--;
        if (freq[x] == 0) d--;
    };

    std::vector<bool> ans(q);
    int cur_l = 0;
    int cur_r = -1;

    for (const Query& qq : qs) {
        while (cur_l > qq.l) {
            --cur_l;
            add(cur_l);
        }
        while (cur_r < qq.r) {
            ++cur_r;
            add(cur_r);
        }
        while (cur_l < qq.l) {
            remove(cur_l);
            cur_l++;
        }
        while (cur_r > qq.r) {
            remove(cur_r);
            cur_r--;
        }
        ans[qq.idx] = (d == k && odd == 0);
    }
    return ans;
}