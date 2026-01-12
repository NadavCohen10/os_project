#pragma once
#include <vector>
using namespace std;

class Graph {
    int V; // number of vertices
    vector<vector<int>> adjMatrix;
    bool isDirected;

public:
    Graph(int V, bool isDirected = false);

    // Add edge between u and v
    void addEdge(int u, int v, int weight = 1);

    // Remove edge between u and v
    void removeEdge(int u, int v);

    // Print matrix
    void printGraph();

    // Degree of a vertex
    int degree(int u);

    // Get number of vertices
    int vertexCount() const { return V; }

    // Get adjacency matrix
    const vector<vector<int>>& getMatrix() const { return adjMatrix; }

    // Check if the graph is directed
    bool directed() const { return isDirected; }
};
