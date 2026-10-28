# Segment trees in C++

This example implements two recursive segment-tree searches in [`segment_trees.cpp`](segment_trees.cpp). Both trees take `O(n)` time and `O(n)` additional space to build, and each search takes `O(log n)` time.

## 1. Searching for an array prefix with a given sum

Given an array of non-negative integers and a value `x`, find the smallest prefix length `i` such that:

```text
a[0] + a[1] + ... + a[i-1] >= x
```

`SegmentTree` stores the sum of each segment. Instead of using binary search with `O(log n)` prefix-sum queries, which costs `O(log² n)` per search, `firstPrefixAtLeast` descends the tree once:

- If the left child's sum is at least `x`, search the left half.
- Otherwise, subtract that sum from `x` and search the right half.
- At a leaf, return its zero-based index plus one as the prefix length.

The method returns `0` when `x <= 0`, and `-1` when the total sum is less than `x`. Prefix sums are stored as `long long` to accommodate larger totals.

## 2. Searching for the first element greater than a given value

Given a value `x` and a half-open range `[l, r)`, find the smallest zero-based index `i` in that range such that `a[i] > x`.

`MaxSegmentTree` stores the maximum of each segment. Its recursive `firstGreaterThan` search:

- Skips nodes outside the query range or whose maximum is at most `x`.
- Searches the left child before the right child, ensuring the smallest index is found.
- Returns the leaf index when it finds a qualifying element.

It returns `-1` for an empty or invalid range and when no qualifying element exists.

## Build and run

Requires a C++17 compiler:

```bash
c++ -std=c++17 -Wall -Wextra -pedantic segment_trees.cpp -o segment_trees
./segment_trees
```
