package main

import "fmt"

type SegmentTree struct {
	sums []int
	n    int
}

func NewSegmentTree(values []int) *SegmentTree {
	tree := &SegmentTree{n: len(values)}
	if tree.n == 0 {
		return tree
	}

	tree.sums = make([]int, 4*tree.n)
	tree.build(1, 0, tree.n-1, values)
	return tree
}

func (tree *SegmentTree) build(node, left, right int, values []int) {
	if left == right {
		tree.sums[node] = values[left]
		return
	}

	middle := left + (right-left)/2
	tree.build(2*node, left, middle, values)
	tree.build(2*node+1, middle+1, right, values)
	tree.sums[node] = tree.sums[2*node] + tree.sums[2*node+1]
}

// FirstPrefixAtLeast returns the smallest i for which a[0]+...+a[i-1] >= x.
// It returns 0 when x <= 0 and -1 when the total sum is smaller than x.
func (tree *SegmentTree) FirstPrefixAtLeast(x int) int {
	if x <= 0 {
		return 0
	}
	if tree.n == 0 || tree.sums[1] < x {
		return -1
	}

	return tree.firstPrefixAtLeast(1, 0, tree.n-1, x)
}

func (tree *SegmentTree) firstPrefixAtLeast(node, left, right, x int) int {
	if left == right {
		return left + 1
	}

	middle := left + (right-left)/2
	leftChild := 2 * node
	if tree.sums[leftChild] >= x {
		return tree.firstPrefixAtLeast(leftChild, left, middle, x)
	}

	return tree.firstPrefixAtLeast(leftChild+1, middle+1, right, x-tree.sums[leftChild])
}

func main() {
	values := []int{2, 1, 4, 0, 3}
	tree := NewSegmentTree(values)

	for _, x := range []int{0, 1, 3, 7, 8, 10, 11} {
		fmt.Printf("x = %2d, first prefix length = %d\n", x, tree.FirstPrefixAtLeast(x))
	}
}
