package main

import "fmt"

// Knuth-Morris-Pratt Algorithm
func pi(s string) []int {
	n := len(s)
	piS := make([]int, n)
	j := 0
	for i := 1; i < n; i++ {
		for j > 0 && s[j] != s[i] {
			j = piS[j-1]
		}
		if s[i] == s[j] {
			j++
		}
		piS[i] = j
	}
	return piS
}

// string matching
// Given a string and a pattern
// count the number of positions where
// the pattern occurs in the string
func stringMatching(t string, p string) int {
	s := p + "#" + t
	piS := pi(s)
	ans := 0
	m := len(p)
	for _, v := range piS {
		if v == m {
			ans++
		}
	}
	return ans
}

func periodOfWords(n int, s string) int64 {
	pi := make([]int32, n)
	for i := 1; i < n; i++ {
		j := pi[i-1]
		for j > 0 && s[i] != s[j] {
			j = pi[j-1]
		}
		if s[i] == s[j] {
			j++
		}
		pi[i] = j
	}
	shortest := make([]int32, n)
	var ans int64
	for i := 0; i < n; i++ {
		if pi[i] > 0 {
			if shortest[pi[i]-1] > 0 {
				shortest[i] = shortest[pi[i]-1]
			} else {
				shortest[i] = pi[i]
			}
			ans += int64(i+1) - int64(shortest[i])
		}
	}
	return ans
}

func main() {
	fmt.Println("Hello, strings!")
	fmt.Println(stringMatching("saippuakauppias", "pp"))
	fmt.Println(periodOfWords(8, "babababa"))
}
