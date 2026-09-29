use std::collections::VecDeque;
use std::io::{self, Read};

fn max_flow(adj: &[Vec<usize>], capacity: &mut [Vec<i64>], source: usize, sink: usize) -> i64 {
    let n = adj.len();
    let mut parent = vec![usize::MAX; n];

    // Find a way from the source to sink on a path with positive capacities
    let reachable = |parent: &mut [usize], capacity: &[Vec<i64>]| -> bool {
        let mut q = VecDeque::new();
        q.push_back(source);
        while let Some(node) = q.pop_front() {
            for &son in &adj[node] {
                let w = capacity[node][son];
                if w <= 0 || parent[son] != usize::MAX {
                    continue;
                }
                parent[son] = node;
                q.push_back(son);
            }
        }
        parent[sink] != usize::MAX
    };

    let mut flow = 0i64;
    // While there is a way from source to sink with positive capacities
    while reachable(&mut parent, capacity) {
        let mut node = sink;
        // The minimum capacity on the path from source to sink
        let mut curr_flow = i64::MAX;
        while node != source {
            curr_flow = curr_flow.min(capacity[parent[node]][node]);
            node = parent[node];
        }
        node = sink;
        while node != source {
            // Subtract the capacity from capacity edges
            capacity[parent[node]][node] -= curr_flow;
            // Add the current flow to flow backedges
            capacity[node][parent[node]] += curr_flow;
            node = parent[node];
        }
        flow += curr_flow;
        parent.fill(usize::MAX);
    }

    flow
}

fn main() {
    let mut input = String::new();
    io::stdin().read_to_string(&mut input).unwrap();
    let mut iter = input.split_whitespace().map(|t| t.parse::<i64>().unwrap());

    let n = iter.next().unwrap() as usize;
    let m = iter.next().unwrap() as usize;

    let mut capacity = vec![vec![0i64; n]; n];
    let mut adj = vec![Vec::new(); n];
    for _ in 0..m {
        let a = iter.next().unwrap() as usize - 1;
        let b = iter.next().unwrap() as usize - 1;
        let c = iter.next().unwrap();
        adj[a].push(b);
        adj[b].push(a);
        capacity[a][b] += c;
    }

    println!("{}", max_flow(&adj, &mut capacity, 0, n - 1));
}
