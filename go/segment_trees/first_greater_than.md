# Searching for the first element greater than a given value

## Problem

Given an array `a`, a value `x`, and a half-open range `[l, r)`, find the smallest index `i` such that:

```text
l <= i < r and a[i] > x
```

Return `-1` if no such element exists. Indices are zero-based.

For example, with `a = [4, 1, 7, 3, 7, 2]`, `x = 4`, and range `[1, 6)`, the answer is `2` because `a[2] = 7` is the first value greater than `4` in that range.

## Solution

A segment tree can store the maximum value in each array segment. One approach is to binary-search the answer's position and use range-maximum queries to test each candidate prefix. This takes `O(log² n)` time per search.

Instead, descend the tree directly. At each node:

1. Discard the node if its segment does not intersect the query range.
2. Discard the node if its maximum is at most `x`; none of its elements can be the answer.
3. Otherwise, search the left child first. If it contains no answer, search the right child.
4. Return the index when reaching a qualifying leaf.

Searching the left child before the right child ensures that the first leaf found has the smallest index. Both the pruning and the recursive descent are implemented in `FirstGreaterThan`.

### Edge cases

- An empty or invalid range returns `-1`.
- An empty array returns `-1`.
- A range whose maximum is at most `x` returns `-1`.

### Complexity

- Building the tree: `O(n)` time.
- Each search: `O(log n)` time.
- Additional space: `O(n)`.

## Go implementation

The complete implementation is in [`first_greater_than.go`](first_greater_than.go). Run both segment-tree examples from this directory with:

```bash
go run .
```
