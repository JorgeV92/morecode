#include <iostream>
#include <vector>
#include <string>

bool canMakeSubsequence(std::string s, std::string t) {
    int m = s.size(), n = t.size();
    if (m > n) return false;
    std::vector<int> L(m+1);
    for (int i = 0; i < m; i++) {
        int j = L[i];
        while (j < n && t[j] != s[i]) j++;
        L[i+1] = j+1;
    }
    if (L[m] <= n) return true;
    std::vector<int> R(m+1, n);
    for (int i = m-1; i>= 0; i--) {
        int j = R[i+1]-1;
        while (j >= 0 && t[j] != s[i]) j--;
        R[i] = j;
    }
    for (int i = 0; i < m; i++) 
        if (L[i] < R[i+1])
            return true;
    return false;
}