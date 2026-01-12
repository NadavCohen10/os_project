#include "graph.h"
#include <iostream>
using namespace std;

Graph::Graph(int V, bool isDirected) : V(V), adjMatrix(V, vector<int>(V, 0)), isDirected(isDirected) {}

// Add edge between u and v
// If the graph is undirected, also add the edge from v to u
bool Graph::addEdge(int u, int v, int weight) {
    if (u < 0 || u >= V || v < 0 || v >= V) {
        cerr << "Error: Vertex index out of bounds." << endl;
        return false;
    }
    if (u == v) {
        cerr << "Error: Cannot add an edge from a vertex to itself." << endl;
        return false;
    }
    if (adjMatrix[u][v] == weight) {
        cerr << "Error: Edge already exists between " << u << " and " << v << "." << endl;
        return false;
    }
    // If the edge already exists, update its weight
    adjMatrix[u][v] = weight;
    if (!isDirected)
        adjMatrix[v][u] = weight; // if undirected graph

    return true;

}

// Remove edge between u and v
void Graph::removeEdge(int u, int v) {
    if (u < 0 || u >= V || v < 0 || v >= V) {
        cerr << "Error: Vertex index out of bounds." << endl;
        return;
    }
    if (adjMatrix[u][v] == 0) {
        cerr << "Error: No edge exists between " << u << " and " << v << "." << endl;
        return;
    }
    adjMatrix[u][v] = 0;
    if (!isDirected)
        adjMatrix[v][u] = 0; // if undirected graph
  
}

// Print the adjacency matrix of the graph
void Graph::printGraph() {
    cout << "Adjacency Matrix:" << endl;
    for (int i = 0; i < V; i++) {
        cout << i << ": ";
        for (int j = 0; j < V; j++) {
            cout << adjMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

// Get the degree of a vertex
int Graph::degree(int u) {
    int deg = 0;
    for (int j = 0; j < V; j++) {
        if (adjMatrix[u][j] != 0) // if there is an edge between u and j with non-zero weight
            deg++;
    }
    return deg;
}
