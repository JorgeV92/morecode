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
                    self.preds[v].append(v)

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

    def _semi_path_exits(self, u, v):
        treshold = self.pre[v]
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
                if self.pre[y] >= treshold:
                    seen.add(y)
                    stack.append(y)
        return True 

    def _compute_sdom(self):
        sdom = {self.s: None}
        for v in self.order:
            if v == self.s:
                continue
            for u in self.order:
                if self.pre[u] >= self.pre[v]:
                    continue
                if self._semi_path_exits(u, v):
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
        pass  
        