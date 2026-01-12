// ===== Algorithms.h =====
#pragma once
#include <vector>
using namespace std;
class Graph;

class algoSCC
{
public:

    // 2) SCC – returns a vector comp where comp[v] is the component id (0..k-1).
    // For undirected graphs, this is equivalent to connected components.
    static vector<int> scc(const Graph &g);

private:

    // SCC helpers (Kosaraju)
    static void dfs1(const vector<vector<int>> &m, int u, vector<int> &vis, vector<int> &order);
    static void dfs2(const vector<vector<int>> &tr, int u, int cid, vector<int> &comp);

};