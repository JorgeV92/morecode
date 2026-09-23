def quick_stair(P, l, r):
    if l == r: return [P[l]]
    mid = l + (r - l) // 2
    L = quick_stair(P, l, mid)
    R = quick_stair(P, mid + 1, r)
    while L and L[-1][1] <= R[0][1]: L.pop()
    return L + R

def staircase(X, Y):
    n = len(X)
    if n == 0: return []
    P = [(X[i], Y[i]) for i in range(n)]
    P.sort(key=lambda p: p[0])
    return quick_stair(P, 0, n - 1)

def test_pareto():
    X = [1, 2, 3, 4, 5, 6, 7]
    Y = [10, 8, 6, 3, 7, 5, 2]

    pareto = staircase(X, Y)

    print("Pareto-optimal points:")

    for x, y in pareto:
        print(x, y)