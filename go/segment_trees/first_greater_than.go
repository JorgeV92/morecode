package main

type MaxSegmentTree struct {
	maxima []int
	n      int
}

func NewMaxSegmentTree(values []int) *MaxSegmentTree {
	tree := &MaxSegmentTree{n: len(values)}
	if tree.n == 0 {
		return tree
	}

	tree.maxima = make([]int, 4*tree.n)
	tree.build(1, 0, tree.n-1, values)
	return tree
}

func (tree *MaxSegmentTree) build(node, left, right int, values []int) {
	if left == right {
		tree.maxima[node] = values[left]
		return
	}

	middle := left + (right-left)/2
	tree.build(2*node, left, middle, values)
	tree.build(2*node+1, middle+1, right, values)
	tree.maxima[node] = max(tree.maxima[2*node], tree.maxima[2*node+1])
}

// FirstGreaterThan returns the first index i in [left, right) where values[i] > x.
// It returns -1 when no such element exists or the range is invalid.
func (tree *MaxSegmentTree) FirstGreaterThan(left, right, x int) int {
	if tree.n == 0 || left < 0 || right > tree.n || left >= right {
		return -1
	}

	return tree.firstGreaterThan(1, 0, tree.n-1, left, right, x)
}

func (tree *MaxSegmentTree) firstGreaterThan(node, left, right, queryLeft, queryRight, x int) int {
	if right < queryLeft || left >= queryRight || tree.maxima[node] <= x {
		return -1
	}
	if left == right {
		return left
	}

	middle := left + (right-left)/2
	if index := tree.firstGreaterThan(2*node, left, middle, queryLeft, queryRight, x); index != -1 {
		return index
	}

	return tree.firstGreaterThan(2*node+1, middle+1, right, queryLeft, queryRight, x)
}
