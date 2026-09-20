import math

def validSubarrays(self, nums: list[int], k: int, queries: list[list[int]]) -> list[bool]:
    n = len(nums)
    q = len(queries)
    block_size = int(math.sqrt(n)) + 1 

    class Query:
        def __init__(self,l,r,i):
            self.l = l
            self.r = r
            self.idx = i 

    qs = [Query(queries[i][0], queries[i][1], i) for i in range(q)] 
    qs.sort(key=lambda x: (x.l // block_size, x.r)) # fix sort 
    freq = [0] * (100001)
    d = 0 
    odd = 0 

    def add(idx):
        nonlocal d, odd
        x = nums[idx]
        if freq[x] == 0: d += 1
        if freq[x] % 2 == 0: odd += 1
        else: odd -= 1
        freq[x] += 1

    def remove(idx): 
        nonlocal d, odd
        x = nums[idx]
        if freq[x] % 2 == 0: odd += 1
        else: odd -= 1
        freq[x] -= 1
        if freq[x] == 0: d -= 1

    ans = [False] * q
    cur_l = 0
    cur_r = - 1

    for qq in qs:
        while cur_l > qq.l:
            cur_l -= 1
            add(cur_l)
        while cur_r < qq.r:
            cur_r += 1
            add(cur_r)
        while cur_l < qq.l:
            remove(cur_l)
            cur_l += 1
        while cur_r > qq.r:
            remove(cur_r)
            cur_r -= 1
        ans[qq.idx] = (d == k and odd == 0)

    return ans

    


        