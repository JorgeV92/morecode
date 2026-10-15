package main

import "fmt"

// Go translations of the two DFS implementations at:
// https://cp-algorithms.com/graph/depth-first-search.html#implementation
// Source: cp-algorithms contributors, CC BY-SA 4.0.
// Vertices are numbered from 0 to len(adj)-1.
var adj [][]int

// Basic DFS: mark every vertex reachable from v.
var visited []bool

func dfs(v int) {
	visited[v] = true
	for _, u := range adj[v] {
		if !visited[u] {
			dfs(u)
		}
	}
}

// DFS with colors and entry/exit times.
// Colors: 0 = unvisited, 1 = entered, 2 = exited.
var color []int
var timeIn, timeOut []int
var dfsTimer int

func dfsWithTimes(v int) {
	timeIn[v] = dfsTimer
	dfsTimer++
	color[v] = 1
	for _, u := range adj[v] {
		if color[u] == 0 {
			dfsWithTimes(u)
		}
	}
	color[v] = 2
	timeOut[v] = dfsTimer
	dfsTimer++
}

func main() {
	// An undirected graph with a cycle and an isolated vertex (4).
	adj = [][]int{
		{1, 2},
		{0, 2},
		{0, 1, 3},
		{2},
		{},
	}
	n := len(adj)

	visited = make([]bool, n)
	dfs(0)
	fmt.Println("Visited from 0:", visited)

	color = make([]int, n)
	timeIn = make([]int, n)
	timeOut = make([]int, n)
	dfsTimer = 0
	dfsWithTimes(0)
	fmt.Println("Colors from 0:", color)
	fmt.Println("Entry times:", timeIn)
	fmt.Println("Exit times:", timeOut)

	// To cover disconnected graphs, start DFS at each unvisited vertex.
	// Keep the existing state and timer between component traversals.
	for v := 0; v < n; v++ {
		if !visited[v] {
			dfs(v)
		}
		if color[v] == 0 {
			dfsWithTimes(v)
		}
	}
	fmt.Println("Visited across all components:", visited)
	fmt.Println("Colors across all components:", color)
}
