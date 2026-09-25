class BIT:
    def __init__(self, n):
        self.n = n
        self.t = [0] * (n+1)
    def update(self, i, v):
        i += 1
        while i <= self.n:
            if v > self.t[i]: self.t[i] = v
            i += i & -i
    def query(self, i):
        i += 1
        r = 0
        while i > 0:
            if self.t[i] > r: r = self.t[i]
            i -= i & -i
        return r 


def maxAlternatingSum(self, nums: list[int], k: int) -> int:
    n = len(nums)
    vals = sorted(set(nums))
    comp = {v: i for i, v in enumerate(vals)}
    m = len(vals)
    bitLow = BIT(m)
    bitHigh = BIT(m)
    low = [0] * n
    high = [0] * n
    ans = 0
    for j in range(n):
        if j >= k:
            i = j-k
            ci = comp[nums[i]]
            bitLow.update(ci, low[i])
            bitHigh.update(m-1-ci, high[i]) 
        cj = comp[nums[j]]
        high[j] = nums[j] + (bitLow.query(cj-1) if cj > 0 else 0)
        rev = m - 1 - cj
        low[j] = nums[j] + (bitHigh.query(rev-1) if rev > 0 else 0)
        ans = max(ans, low[j], high[j])
    return ans