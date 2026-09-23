class DominatorAnalyzer:
    def __init__(self, g, s):
        vertices = set(g)
        for neighbors in g.values():
            vertices.update(neighbors)
        self.g = {u: list(g.get(u, [])) for u in vertices}
        self.s = s

        self.tree = {u : [] for u in vertices}
        self.parent = {}
        self.pre = {}
        self.order = []

        self._build_dfs_tree()
        self.reachable = set(self.order)

        self.preds = {u: [] for u in self.order}

        for u in self.order:
            for v in self.g[u]:
                if v in self.reachable:
                    self.preds[v].append(u)

        self.dom = self._compute_dominators()
        self.idom = self._compute_idom_from_dom()
        self.sdom = self._compute_sdom()
        self.rdom = self._compute_rdom()

        self.idom_via_sdom = self._compute_idom_from_sdom_rdom()

    def _build_dfs_tree(self):
        vis = set()

        def dfs(u, p=None):
            vis.add(u)
            self.parent[u] = p
            self.pre[u] = len(self.order) + 1
            self.order.append(u)

            for v in self.g[u]:
                if v not in vis:
                    self.tree[u].append(v)
                    dfs(v, u)
        dfs(self.s)

    def _compute_dominators(self):
        nodes = self.order 
        all_nodes = set(nodes)
        dom = {}
        for v in nodes:
            if v == self.s:
                dom[v] = {v}
            else:
                dom[v] = set(all_nodes)
        changed = True  

        while changed:
            changed = False 
            for v in nodes:
                if v == self.s:
                    continue
                predecessors = self.preds[v]
                common = set(dom[predecessors[0]])
                for p in predecessors[1:]:
                    common &= dom[p] 
                new_dom = {v} | common
                if new_dom != dom[v]:
                    dom[v] = new_dom
                    changed = True 
        return dom

    def _compute_idom_from_dom(self):
        idom = {self.s: None} 
        for v in self.order:
            if v == self.s:
                continue
            strict = self.dom[v] - {v}
            for d in strict:
                if all(x in self.dom[d] for x in strict if x != d):
                    idom[v] = d 
                    break
        return idom

    def _semi_path_exists(self, u, v):
        threshold = self.pre[v]
        stack = [u]
        seen = {u}
        while stack:
            x = stack.pop()
            if x == v: return True 
            for y in self.g[x]:
                if y not in self.reachable: 
                    continue
                if y in seen:
                    continue
                if self.pre[y] >= threshold:
                    seen.add(y)
                    stack.append(y)
        return False

    def _compute_sdom(self):
        sdom = {self.s: None}
        for v in self.order:
            if v == self.s:
                continue
            for u in self.order:
                if self.pre[u] >= self.pre[v]:
                    break
                if self._semi_path_exists(u, v):
                    sdom[v] = u 
                    break 
        return sdom

    def _tree_path(self, ancestor, v):
        path = []
        x = v 
        while x != ancestor:
            path.append(x)
            x = self.parent[x] 
            if x is None:
                raise ValueError(f"{ancestor} is not an ancestor of {v}")
        path.append(ancestor)
        path.reverse()
        return path 

    def _compute_rdom(self):
        rdom = {self.s: None} 
        for v in self.order:
            if v == self.s:
                continue
            sd = self.sdom[v]
            path = self._tree_path(sd, v)
            candidates = path[1:]
            rdom[v] = min(candidates, key=lambda x: self.pre[self.sdom[x]])
        return rdom 

    def _compute_idom_from_sdom_rdom(self):
        idom = {self.s: None} 
        for v in self.order:
            if v == self.s:
                continue
            if self.rdom[v] == v:
                idom[v] = self.sdom[v]
            else:
                idom[v] = idom[self.rdom[v]] 
        return idom


class LengauerTarjan:
    def __init__(self, n):
        self.n = n
        self.g = [[] for _ in range(n + 1)]
        self.arr = [0] * (n + 1)
        self.rev = [0] * (n + 1)
        self.parent = [0] * (n + 1)
        self.sdom = [0] * (n + 1)
        self.idom = [0] * (n + 1)
        self.rg = [[] for _ in range(n + 1)]
        # bucket[x] contains vertices whose
        # semidominator is x.
        self.bucket = [[] for _ in range(n + 1)]
        # Union-find / eval-link structure.
        self.ancestor = [0] * (n + 1)
        self.best = [0] * (n + 1)
        self.timer = 0

    def add_edge(self, u, v):
        self.g[u].append(v)
    
    def _dfs(self, u):
        self.timer += 1
        self.arr[u] = self.timer
        self.rev[self.timer] = u
        self.sdom[self.timer] = self.timer
        self.best[self.timer] = self.timer
        for v in self.g[u]:
            if self.arr[v] == 0:
                self._dfs(v)
                self.parent[self.arr[v]] = self.arr[u]
            if self.arr[v] != 0:
                self.rg[self.arr[v]].append(self.arr[u])

    def _compress(self, v):
        if self.ancestor[self.ancestor[v]] != 0:
            self._compress(self.ancestor[v])
            parent_best = self.best[self.ancestor[v]]
            if self.sdom[parent_best] < self.sdom[self.best[v]]:
                self.best[v] = parent_best
            self.ancestor[v] = self.ancestor[self.ancestor[v]]

    def _eval(self, v):
        if self.ancestor[v] == 0:
            return self.best[v]
        self._compress(v)
        parent_best = self.best[self.ancestor[v]]
        if self.sdom[parent_best] < self.sdom[self.best[v]]:
            return parent_best

        return self.best[v]

    def _link(self, parent, child):
        self.ancestor[child] = parent

    def compute(self, start):
        self._dfs(start)
        N = self.timer

        for i in range(N, 1, -1):
            for p in self.rg[i]:
                u = self._eval(p)
                self.sdom[i] = min(self.sdom[i], self.sdom[u])
            self.bucket[self.sdom[i]].append(i)
            self._link(self.parent[i], i)
            for v in self.bucket[self.parent[i]]:
                u = self._eval(v)
                if self.sdom[u] == self.sdom[v]:
                    self.idom[v] = self.sdom[v]
                else:
                    self.idom[v] = u
            self.bucket[self.parent[i]].clear()
        for i in range(2, N + 1):
            if self.idom[i] != self.sdom[i]:
                self.idom[i] = self.idom[self.idom[i]]
        result = [-1] * (self.n + 1)
        result[start] = -1
        for i in range(2, N + 1):
            vertex = self.rev[i]
            dominator = self.rev[self.idom[i]]
            result[vertex] = dominator
        return result 


def critical_nodes(n, edges):
    rev_adj = [[] for _ in range(n)]
    for a, b in edges:
        a -= 1
        b -= 1
        rev_adj[b].append(a)
    all_cities = set(range(n))
    dom = [set(all_cities) for _ in range(n)]
    dom[0] = {0}

    while True:
        changed = False
        new_dom = [set() for _ in range(n)]
        new_dom[0] = {0}
        for city in range(1, n):
            updated = set(all_cities)
            for prev in rev_adj[city]:
                updated &= dom[prev]
            updated.add(city)
            if updated != dom[city]:
                changed = True
            new_dom[city] = updated
        dom = new_dom
        if not changed:
            break
        
    critical = []

    for city in range(n):
        if city in dom[n - 1]:
            critical.append(city + 1)

    return critical

def test_lt():
    lt = LengauerTarjan(9)
    lt.add_edge(1, 2)
    lt.add_edge(1, 3)
    lt.add_edge(2, 3)
    lt.add_edge(2, 5)
    lt.add_edge(2, 9)
    lt.add_edge(3, 4)
    lt.add_edge(4, 2)
    lt.add_edge(5, 6)
    lt.add_edge(5, 8)
    lt.add_edge(6, 7)
    lt.add_edge(6, 3)
    lt.add_edge(7, 1)
    lt.add_edge(7, 4)
    lt.add_edge(7, 5)
    lt.add_edge(8, 7)
    lt.add_edge(9, 5)
    lt.add_edge(9, 8)

    idom = lt.compute(1)

    for v in range(1, 10):
        print(f"idom({v}) =", idom[v])