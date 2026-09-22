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

        self.pred = {u: [] for u in self.order}

        for u in self.order:
            for v in self.g[u]:
                if v in self.reachable:
                    self.pred[v].append(v)

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
        pass 

    def _compute_idom_from_dom():
        pass 

    def _compute_sdom():
        pass 

    def _compute_rdom():
        pass  
        