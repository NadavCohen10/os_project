#pragma once
#include <vector>
using namespace std;
class Graph;

class Algorithms {
public:

    // Returns true if the graph has an Euler circuit
    static bool hasEulerCircuit(const Graph& g);

    
    // Returns the Euler circuit as a vector of vertices, or an empty vector if none exists
    static vector<int> eulerCircuit(const Graph& g);

private:
    // Helper function for DFS traversal
    static void dfsOnEdges(const vector<vector<int>>& m, int u, vector<int>& vis);
};
