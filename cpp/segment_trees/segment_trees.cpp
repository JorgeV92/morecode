#include <algorithm>
#include <iostream>
#include <vector>

class SegmentTree {
public:
    explicit SegmentTree(const std::vector<int>& values)
        : n(static_cast<int>(values.size())), sums(4 * values.size(), 0) {
        if (n > 0) {
            build(1, 0, n - 1, values);
        }
    }

    // Returns the smallest i for which values[0] + ... + values[i-1] >= x.
    // Returns 0 when x <= 0 and -1 when the total sum is smaller than x.
    int firstPrefixAtLeast(long long x) const {
        if (x <= 0) {
            return 0;
        }
        if (n == 0 || sums[1] < x) {
            return -1;
        }
        return firstPrefixAtLeast(1, 0, n - 1, x);
    }

private:
    int n;
    std::vector<long long> sums;

    void build(int node, int left, int right, const std::vector<int>& values) {
        if (left == right) {
            sums[node] = values[left];
            return;
        }

        int middle = left + (right - left) / 2;
        build(2 * node, left, middle, values);
        build(2 * node + 1, middle + 1, right, values);
        sums[node] = sums[2 * node] + sums[2 * node + 1];
    }

    int firstPrefixAtLeast(int node, int left, int right, long long x) const {
        if (left == right) {
            return left + 1;
        }

        int middle = left + (right - left) / 2;
        int leftChild = 2 * node;
        if (sums[leftChild] >= x) {
            return firstPrefixAtLeast(leftChild, left, middle, x);
        }
        return firstPrefixAtLeast(leftChild + 1, middle + 1, right, x - sums[leftChild]);
    }
};

class MaxSegmentTree {
public:
    explicit MaxSegmentTree(const std::vector<int>& values)
        : n(static_cast<int>(values.size())), maxima(4 * values.size(), 0) {
        if (n > 0) {
            build(1, 0, n - 1, values);
        }
    }

    // Returns the first index i in [left, right) where values[i] > x.
    // Returns -1 when no such element exists or the range is invalid.
    int firstGreaterThan(int left, int right, int x) const {
        if (n == 0 || left < 0 || right > n || left >= right) {
            return -1;
        }
        return firstGreaterThan(1, 0, n - 1, left, right, x);
    }

private:
    int n;
    std::vector<int> maxima;

    void build(int node, int left, int right, const std::vector<int>& values) {
        if (left == right) {
            maxima[node] = values[left];
            return;
        }

        int middle = left + (right - left) / 2;
        build(2 * node, left, middle, values);
        build(2 * node + 1, middle + 1, right, values);
        maxima[node] = std::max(maxima[2 * node], maxima[2 * node + 1]);
    }

    int firstGreaterThan(int node, int left, int right, int queryLeft, int queryRight, int x) const {
        if (right < queryLeft || left >= queryRight || maxima[node] <= x) {
            return -1;
        }
        if (left == right) {
            return left;
        }

        int middle = left + (right - left) / 2;
        int index = firstGreaterThan(2 * node, left, middle, queryLeft, queryRight, x);
        if (index != -1) {
            return index;
        }
        return firstGreaterThan(2 * node + 1, middle + 1, right, queryLeft, queryRight, x);
    }
};

int main() {
    SegmentTree sumTree({2, 1, 4, 0, 3});
    for (int x : {0, 1, 3, 7, 8, 10, 11}) {
        std::cout << "x = " << x << ", first prefix length = "
                  << sumTree.firstPrefixAtLeast(x) << '\n';
    }

    MaxSegmentTree maxTree({4, 1, 7, 3, 7, 2});
    std::cout << "First element greater than 4 in [1, 6): "
              << maxTree.firstGreaterThan(1, 6, 4) << '\n';
    std::cout << "First element greater than 6 in [3, 6): "
              << maxTree.firstGreaterThan(3, 6, 6) << '\n';
    std::cout << "First element greater than 7 in [0, 6): "
              << maxTree.firstGreaterThan(0, 6, 7) << '\n';
}
