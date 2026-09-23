#pragma once 

#include <algorithm>
#include <iostream>
#include <vector>

struct Point {
    int x;
    int y;
};

std::vector<Point> quickStair(const std::vector<Point>& P, int l, int r) {
    if (l == r) {
        return {P[l]};
    }
    int mid = l + (r - l) / 2;
    std::vector<Point> L = quickStair(P, l, mid);
    std::vector<Point> R = quickStair(P, mid + 1, r);
    while (!L.empty() && L.back().y <= R[0].y) {
        L.pop_back();
    }
    std::vector<Point> result;
    result.reserve(L.size() + R.size());
    for (const Point& p : L) {
        result.push_back(p);
    }
    for (const Point& p : R) {
        result.push_back(p);
    }
    return result;
}

std::vector<Point> staircase(std::vector<int>& X, std::vector<int>& Y) {
    int n = X.size();
    std::vector<Point> P(n);
    for (int i = 0; i < n; ++i) {
        P[i] = {X[i], Y[i]};
    }
    sort(P.begin(), P.end(),
         [](const Point& a, const Point& b) {
             return a.x < b.x;
         });

    if (P.empty()) {
        return {};
    }
    return quickStair(P, 0, n - 1);
}

void testPareto() {
    std::vector<int> X = {1, 2, 3, 4, 5, 6, 7};
    std::vector<int> Y = {10, 8, 6, 3, 7, 5, 2};

    std::vector<Point> pareto = staircase(X, Y);

    std::cout << "Pareto-optimal points:\n";

    for (const Point& p : pareto) {
        std::cout << "(" << p.x << ", " << p.y << ")\n";
    }
}