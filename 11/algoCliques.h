// ===== algoCliques.h =====
#pragma once
#include <vector>
using namespace std;
class Graph;

class algoCliques
{
public:

    // 3) Number of cliques in an undirected graph (counts all cliques of size >= 1).
    static long long countCliques(const Graph &g);

    // 4) Maximum clique (returns the vertex indices of a maximum clique).
    static vector<int> maxClique(const Graph &g);

private:

    // Bron–Kerbosch for cliques
    static void bronKerbosch(const vector<vector<int>> &adj, vector<int> &R, vector<int> &P, vector<int> &X,
                             long long &count, vector<int> &best);
};