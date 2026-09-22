#pragma once 

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <stdexcept>

class DominatorAnalyzer {
private:
    int n;
    int s;
    std::vector<std::vector<int>> g;
    // dfs 
    std::vector<std::vector<int>> tree;
    std::vector<int> parent;
    std::vector<int> pre;
    std::vector<int> order;
    std::vector<bool> vis;
    std::vector<std::vector<int>> preds;
    // dom sets
    std::vector<std::set<int>> dom;
    // dom info 
    std::vector<int> idom;
    std::vector<int> sdom;
    std::vector<int> rdom;
    std::vector<int> idomViaSdom;

    void dfs(int u, int p=-1) {
        vis[u] = true;
        parent[u] = p;
        pre[u] = (int)order.size() + 1;
        order.push_back(u);
        for (int v : g[u]) {
            if (!vis[v]) {
                tree[u].push_back(v);
                dfs(v, u);
            }
        }
    }

    void buildDFSTree() { dfs(s); }

    void buildPredecessors() {
        for (int u : order) {
            for (int v : g[u]) {
                if (pre[v] != 0) 
                    preds[v].push_back(u);
            }
        }
    }

    void computeDominators() {
        std::set<int> allNodes;
        for (int v : order) {
            allNodes.insert(v);
        }
        for (int v : order) {
            if (v == s) dom[v] = {v};
            else dom[v] = allNodes;
        }
        bool changed = true;

        while (changed) {
            changed = false;
            for (int v : order) {
                if (v == s) continue;
                if (preds[v].empty()) continue;
                std::set<int> common = dom[preds[v][0]];
                for (int i = 1; i < (int)preds[v].size(); ++i) {
                    int p = preds[v][i];
                    std::set<int> next;
                    std::set_intersection(
                        common.begin(), common.end(), 
                        dom[p].begin(), dom[p].end(), 
                        std::inserter(next, next.begin()));
                    common = move(next);
                }
                common.insert(v);
                if (common != dom[v]) {
                    dom[v] = move(common);
                    changed = true;
                }
            }
        }
    }

    void computeIdomFromDom() {
        idom[s] = -1;
        for (int v : order) {
            if (v == s) continue;
            std::vector<int> strict;
            for (int d : dom[v]) {
                if (d != v) 
                    strict.push_back(d);
            }
            for (int d : strict) {
                bool valid = true;
                for (int x : strict) {
                    if (x == d) continue;
                    if (!dom[d].count(x)) {
                        valid = false;
                        break;
                    }
                }
                if (valid) {
                    idom[v] = d;
                    break;
                }
            }
        }
    }

    bool semiPathExists(int u, int v) {
        int threshold = pre[v];
        std::vector<int> st; 
        std::vector<bool> seen(n+1, false);
        st.push_back(u);
        seen[u] = true;
        while (!st.empty()) {
            int x = st.back();
            st.pop_back();
            if (x == v) return true;
            for (int y : g[x]) {
                if (pre[y] == 0) continue;
                if (seen[y]) continue;
                if (pre[y] >= threshold) {
                    seen[y] = true;
                    st.push_back(y);
                }
            }
        }
        return false; 
    }

     void computeSdom() {
        sdom[s] = -1;
        for (int v : order) {
            if (v == s)
                continue;
            for (int u : order) {
                if (pre[u] >= pre[v])
                    break;
                if (semiPathExists(u, v)) {
                    sdom[v] = u;
                    break;
                }
            }
        }
    }

    std::vector<int> treePath(int ancestor, int v) {
        std::vector<int> path;
        int x = v;
        while (x != ancestor) {
            path.push_back(x);
            x = parent[x];
            if (x == -1)
                throw std::runtime_error("Vertex is not an ancestor in DFS tree");
        }

        path.push_back(ancestor);
        reverse(path.begin(), path.end());
        return path;
    }

    void computeRdom() {
        rdom[s] = -1;
        for (int v : order) {
            if (v == s)
                continue;
            int sd = sdom[v];
            std::vector<int> path = treePath(sd, v);
            int best = path[1];
            for (int i = 1; i < (int)path.size(); ++i) {
                int x = path[i];
                if (pre[sdom[x]] < pre[sdom[best]]) {
                    best = x;
                }
            }
            rdom[v] = best;
        }
    }

    void computeIdomFromSdomRdom() {
        idomViaSdom[s] = -1;
        for (int v : order) {
            if (v == s)
                continue;
            if (rdom[v] == v) {
                idomViaSdom[v] = sdom[v];
            }
            else {
                idomViaSdom[v] =
                    idomViaSdom[rdom[v]];
            }
        }
    }

public:

     DominatorAnalyzer(const std::vector<std::vector<int>>& graph, int s) : g(graph), s(s) {
        n = (int)g.size() - 1;
        tree.resize(n + 1);
        parent.assign(n + 1, -1);
        pre.assign(n + 1, 0);
        vis.assign(n + 1, false);
        preds.resize(n + 1);
        dom.resize(n + 1);
        idom.assign(n + 1, -1);
        sdom.assign(n + 1, -1);
        rdom.assign(n + 1, -1);
        idomViaSdom.assign(n + 1, -1);
        // Compute everything
        buildDFSTree();

        buildPredecessors();

        computeDominators();

        computeIdomFromDom();

        computeSdom();

        computeRdom();

        computeIdomFromSdomRdom();
    }
};
