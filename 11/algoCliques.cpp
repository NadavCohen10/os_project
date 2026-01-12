// ===== algoCliques.cpp =====
#include "algoCliques.h"
// Include graph header for graph operations
#include "graph.h"
// Include algorithm header for std::sort, std::find, etc.
#include <algorithm>
// Include numeric header for std::iota
#include <numeric>

// ---------- 3) Count cliques & 4) Max clique (Bron–Kerbosch) ----------

// Check if there is an edge between vertices a and b in adjacency matrix
static inline bool isEdge(const vector<vector<int>> &adj, int a, int b) { return adj[a][b] != 0; }

// Bron–Kerbosch recursive algorithm for finding maximal cliques
void algoCliques::bronKerbosch(const vector<vector<int>> &adj, vector<int> &R, vector<int> &P, vector<int> &X,
                              long long &count, vector<int> &best)
{
    // If both P and X are empty, R is a maximal clique
    if (P.empty() && X.empty())
    {
        // Increment count of maximal cliques
        count++; // counting maximal cliques; to count all cliques you would enumerate subsets differently
        // Update best clique if current clique is larger
        if ((int)R.size() > (int)best.size())
            best = R;
        return;
    }
    // Choose a pivot u from P∪X to reduce branches
    int u = -1;
    {
        vector<int> UX = P;
        UX.insert(UX.end(), X.begin(), X.end());
        if (!UX.empty())
            u = UX[0];
    }
    // Compute P \ N(u)
    vector<int> PwithoutNu;
    if (u == -1)
        PwithoutNu = P;
    else
    {
        for (int v : P)
            if (!isEdge(adj, u, v))
                PwithoutNu.push_back(v);
    }
    // Iterate over vertices in P \ N(u)
    for (int v : PwithoutNu)
    {
        // Add v to current clique R
        R.push_back(v);
        // Compute new sets Pn and Xn for recursion
        vector<int> Pn, Xn;
        for (int w : P)
            if (isEdge(adj, v, w))
                Pn.push_back(w);
        for (int w : X)
            if (isEdge(adj, v, w))
                Xn.push_back(w);
        // Recursive call with updated sets
        bronKerbosch(adj, R, Pn, Xn, count, best);
        // Remove v from current clique R
        R.pop_back();
        // Move v from P to X
        P.erase(find(P.begin(), P.end(), v));
        X.push_back(v);
    }
}

// Count the number of maximal cliques in the graph using Bron–Kerbosch algorithm
long long algoCliques::countCliques(const Graph &g)
{
    // We will count MAXIMAL cliques via Bron–Kerbosch (common interpretation in assignments).
    const auto &M = g.getMatrix();
    int n = g.vertexCount();
    // R: current clique, P: candidates, X: excluded
    vector<int> R, P(n), X;
    // Fill P with all vertex indices
    iota(P.begin(), P.end(), 0);
    long long count = 0;
    vector<int> dummy;
    // Run Bron–Kerbosch algorithm
    bronKerbosch(M, R, P, X, count, dummy);
    // Return total count of maximal cliques
    return count;
}

// Find the largest clique in the graph using Bron–Kerbosch algorithm
vector<int> algoCliques::maxClique(const Graph &g)
{
    const auto &M = g.getMatrix();
    int n = g.vertexCount();
    // R: current clique, P: candidates, X: excluded
    vector<int> R, P(n), X;
    // Fill P with all vertex indices
    iota(P.begin(), P.end(), 0);
    long long count = 0;
    vector<int> best;
    // Run Bron–Kerbosch algorithm to find largest clique
    bronKerbosch(M, R, P, X, count, best);
    // Sort the clique vertices for consistent output
    sort(best.begin(), best.end());
    // Return the largest clique found
    return best;
}
