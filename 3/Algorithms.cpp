#include "Algorithms.h"
#include "graph.h"
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

void Algorithms::dfsOnEdges(const vector<vector<int>>& m, int u, vector<int>& vis) {
    int n = (int)m.size(); // Number of vertices in the graph
    vis[u] = 1; // Mark current vertex as visited
    for (int v = 0; v < n; ++v) {
        // If there is an edge from u to v and v is not visited
        if (m[u][v] && !vis[v]) 
            dfsOnEdges(m, v, vis); // Recursively
    }
}

bool Algorithms::hasEulerCircuit(const Graph& g) {
    const auto& M = g.getMatrix(); // Get adjacency matrix of the graph
    int V = g.vertexCount();       // Number of vertices

    // Check if all vertices have even degree
    for (int i = 0; i < V; ++i) {
        int deg = 0;
        for (int j = 0; j < V; ++j) 
            deg += M[i][j]; // Sum degree for vertex i
        if (deg % 2 != 0) 
            return false; // If degree is odd, no Euler circuit
    }

    // Find a starting vertex with degree > 0
    int start = -1;
    for (int i = 0; i < V; ++i) {
        int deg = 0;
        for (int j = 0; j < V; ++j) deg += M[i][j];
        if (deg > 0) { start = i; break; } // First vertex with edges
    }
    if (start == -1) {
        // No edges in the graph, so Euler circuit exists (trivial case)
        return true;
    }

    // Check if all non-zero degree vertices are connected
    vector<int> vis(V, 0); // Visited array
    dfsOnEdges(M, start, vis); // DFS from start vertex
    for (int i = 0; i < V; ++i) {
        int deg = 0;
        for (int j = 0; j < V; ++j) 
            deg += M[i][j];
        if (deg > 0 && !vis[i]) 
            return false; // Not all vertices are connected
    }
    return true; // All conditions for Euler circuit are satisfied
}

vector<int> Algorithms::eulerCircuit(const Graph& g) {
    vector<int> path; // Stores the Euler circuit
    if (!hasEulerCircuit(g)) 
        return path; // If no Euler circuit exists, return empty

    vector<vector<int>> m = g.getMatrix(); // Copy of adjacency matrix to modify
    int V = g.vertexCount(); // Number of vertices

    // Find a starting vertex with degree > 0
    int start = 0, found = -1;
    for (int i = 0; i < V; ++i){
        int deg = 0;
        for (int j = 0; j < V; ++j) 
            deg += m[i][j]; // Calculate degree
        if (deg > 0){ 
            found = i; 
            break; 
        } // First vertex with edges
    }
    if (found != -1) 
        start = found; // Set start vertex

    stack<int> st; // Stack for Hierholzer's algorithm
  

    st.push(start); // Begin from start vertex
    while (!st.empty()) {
        int u = st.top(); // Current vertex

        int v = -1;
        // Find a neighbor v of u with an edge
        for (int j = 0; j < V; ++j) {
            if (m[u][j]) { 
                v = j;
                break; 
            }
        }

        if (v != -1) {
            // Remove the edge u-v from the graph (both directions for undirected)
            m[u][v] = 0;
            m[v][u] = 0;
            st.push(v); // Continue to vertex v
        } else {
            // No more edges from u, add u to path and backtrack
            path.push_back(u);
            st.pop();
        }
    }

    reverse(path.begin(), path.end()); // Reverse to get correct circuit order
    return path; // Return Euler circuit
}
