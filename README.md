# morecode

A collection of algorithms and data structures implemented in C, C++, Go, Python, and Rust. The repository contains both reusable examples and competitive-programming solutions; some problems are solved in multiple languages to compare their implementations.

## Repository layout

| Directory | Contents |
| --- | --- |
| [`c/`](c/) | Fenwick trees, Mo's algorithm, interval processing, prefix sums, and lowest common ancestor using an Euler tour and segment tree. |
| [`cpp/`](cpp/) | Solutions using prefix sums, subsequence matching, divisor grouping, palindrome radii, and Mo's algorithm. |
| [`go/`](go/) | Standalone examples of depth-first search, Knuth–Morris–Pratt string matching, and segment trees. |
| [`python/`](python/) | Fenwick trees, Mo's algorithm, dominator analysis, and Pareto-optimal points. |
| [`rust/`](rust/) | A Cargo project containing an Edmonds–Karp maximum-flow implementation. |

Directories named with numbers, such as `c/520/` or `cpp/509/`, group solutions by problem set. Directories named `data` contain data-structure and algorithm examples outside those problem sets.

## Cross-language examples

- **Maximum alternating sum:** C and Python use coordinate compression with Fenwick trees in `499/D`.
- **Valid subarrays:** C, C++, and Python answer offline range queries with Mo's algorithm in `516/D`.
- **Segment trees:** C uses one for lowest common ancestor queries in `c/data/lca_seg.c`. The [`go/segment_trees/`](go/segment_trees/) examples find the shortest prefix with a given sum and the first element greater than a value within a range, using recursive `O(log n)` searches and detailed explanations.

## Running the standalone examples

Each Go example is its own module:

```bash
cd go/segment_trees
go run .
```

The Rust example reads a graph from standard input:

```bash
cd rust
cargo run --bin download_speed
```

