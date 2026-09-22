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
};
