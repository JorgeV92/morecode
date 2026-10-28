# Searching for an array prefix with a given sum

## Problem

Given an array `a` of non-negative integers and a value `x`, find the smallest prefix length `i` such that:

```text
a[0] + a[1] + ... + a[i-1] >= x
```

The array contains `n` elements, so a valid answer is between `1` and `n` when `x > 0`. The empty prefix has length `0` and sum `0`. If the sum of the entire array is less than `x`, no such prefix exists.

For example, with `a = [2, 1, 4, 0, 3]` and `x = 7`, the answer is `3`, because:

```text
2 + 1 = 3     < 7
2 + 1 + 4 = 7 >= 7
```

## Solution

A segment tree can store the sum of every array segment. One approach is to binary-search the prefix length and ask the segment tree for each prefix sum. Building the tree takes `O(n)`, while each query takes `O(log² n)`: `O(log n)` binary-search steps, each performing an `O(log n)` sum query.

Because all values are non-negative, prefix sums never decrease. I can instead find the answer by descending the segment tree once, reducing each query to `O(log n)`.

Start at the root, which represents the whole array. At each internal node:

1. If the left child's sum is at least `x`, the desired prefix ends in the left half, so move to the left child.
2. Otherwise, the entire left half must be included. Subtract its sum from `x` and move to the right child. The remaining value of `x` is the amount still needed from that half.

Repeat until reaching a leaf. Its zero-based array index is `left`, so the requested prefix length is `left + 1`.

For `a = [2, 1, 4, 0, 3]` and `x = 8`:

- The root's left half sums to `7`. Since `7 < 8`, move right and search for `8 - 7 = 1`.
- In the right half, the left child contains `[0]` and sums to `0`. Move right again, still searching for `1`.
- The leaf contains `3`, so the answer is prefix length `5`.

### Edge cases

- If `x <= 0`, the empty prefix is sufficient, so the answer is `0`.
- If the total sum is less than `x`, return `-1` to indicate that no prefix exists.
- An empty array also returns `-1` for every positive `x`.

### Complexity

- Building the tree: `O(n)` time.
- Each search: `O(log n)` time.
- Additional space: `O(n)`.

## Go implementation

The complete implementation is in [`searching_prefix.go`](searching_prefix.go). It builds a sum segment tree and implements the single-descent search in `FirstPrefixAtLeast`.

For a related example, see [Searching for the first element greater than a given value](first_greater_than.md).

Run the examples with:

```bash
go run .
```
