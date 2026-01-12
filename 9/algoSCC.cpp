// ===== algoSCC.cpp =====
#include "algoSCC.h"
// Include graph header for graph operations
#include "graph.h"
// Include algorithm header for std::max_element, etc.
#include <algorithm>
// Include numeric header for std::iota
#include <numeric>

// ---------- 2) SCC (Kosaraju) ----------

// First DFS to compute finishing order
void algoSCC::dfs1(const vector<vector<int>> &m, int u, vector<int> &vis, vector<int> &order)
{
    // Mark current node as visited
    vis[u] = 1;
    int n = (int)m.size();
    // Visit all neighbors
    for (int v = 0; v < n; ++v)
        if (m[u][v] && !vis[v])
            dfs1(m, v, vis, order);
    // Add node to order after visiting all neighbors
    order.push_back(u);
}

// Second DFS to assign components in transposed graph
void algoSCC::dfs2(const vector<vector<int>> &tr, int u, int cid, vector<int> &comp)
{
    // Assign component id to current node
    comp[u] = cid;
    int n = (int)tr.size();
    // Visit all neighbors in transposed graph
    for (int v = 0; v < n; ++v)
        if (tr[u][v] && comp[v] == -1)
            dfs2(tr, v, cid, comp);
}

// Kosaraju's algorithm to find strongly connected components
vector<int> algoSCC::scc(const Graph &g)
{
    // Get adjacency matrix
    const auto &m = g.getMatrix();
    int n = g.vertexCount();
    // If undirected, SCC == connected components; Kosaraju still works
    vector<int> vis(n, 0), order;
    order.reserve(n);
    // Run first DFS to get finishing order
    for (int i = 0; i < n; ++i)
        if (!vis[i])
            dfs1(m, i, vis, order);
    // Build transposed graph
    vector<vector<int>> tr(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (m[i][j])
                tr[j][i] = 1;
    // Initialize component ids
    vector<int> comp(n, -1);
    int cid = 0;
    // Run second DFS in reverse finishing order to assign components
    for (int i = n - 1; i >= 0; --i)
    {
        int v = order[i];
        if (comp[v] == -1)
        {
            dfs2(tr, v, cid, comp);
            cid++;
        }
    }
    // Return component ids for each node
    return comp; // components are 0..cid-1
}
