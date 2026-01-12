// ===== algoMST.cpp =====
#include "graph.h"
// Include algorithm header for sorting
#include <algorithm>
// Include stack header (not used in this file)
#include <stack>
// Include numeric header for std::iota
#include <numeric>
#include "algoMST.h"

// ---------- 1) MST total weight (Kruskal) ----------

// Anonymous namespace for internal DSU struct
namespace
{
    // Disjoint Set Union (Union-Find) structure
    struct DSU
    {
        // Parent array
        vector<int> p;
        // Rank array
        vector<int> r;
        // Constructor initializes parent and rank
        DSU(int n) : p(n), r(n, 0) { iota(p.begin(), p.end(), 0); }
        // Find with path compression
        int f(int x) { return p[x] == x ? x : p[x] = f(p[x]); }
        // Union by rank
        bool unite(int a, int b)
        {
            a = f(a);
            b = f(b);
            if (a == b)
                return false;
            if (r[a] < r[b])
                swap(a, b);
            p[b] = a;
            if (r[a] == r[b])
                r[a]++;
            return true;
        }
    };
}

// Compute the total weight of the Minimum Spanning Tree using Kruskal's algorithm
long long algoMST::mstWeight(const Graph &g)
{
    // Return -1 if graph is directed (MST only for undirected)
    if (g.directed())
        return -1; // define MST only for undirected here
    // Get adjacency matrix
    const auto &M = g.getMatrix();
    // Get number of vertices
    int n = g.vertexCount();
    // Edge structure for Kruskal
    struct E
    {
        int u, v, w;
    };
    // Vector to store edges
    vector<E> edges;
    edges.reserve(n * n);
    // Collect all edges (only upper triangle for undirected)
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if (M[i][j] != 0)
                edges.push_back({i, j, M[i][j]});
    // If no edges, return 0 for single vertex or -1 for disconnected
    if (edges.empty())
        return n <= 1 ? 0 : -1; // no edges but multiple comps
    // Sort edges by weight
    sort(edges.begin(), edges.end(), [](const E &a, const E &b)
         { return a.w < b.w; });
    // Initialize DSU for Kruskal
    DSU d(n);
    long long total = 0;
    int used = 0;
    // Iterate over edges and add to MST if possible
    for (const auto &e : edges)
    {
        if (d.unite(e.u, e.v))
        {
            total += e.w;
            used++;
            // Stop if MST is complete
            if (used == n - 1)
                break;
        }
    }
    // If not all vertices are connected, return -1
    if (used != n - 1)
        return -1; // not connected
    // Return total weight of MST
    return total;
}