#include <iostream>
#include "graph.h"
#include "Algorithms.h"
#include "getopt.h"
#include <cstdlib>
#include <utility>
#include <random>
#include <algorithm>
using namespace std;

// Print usage instructions and exit
static void usage(const char* prog) {
    cerr << "Usage: " << prog << " -v <vertices> -e <edges> -s <seed>\n"
         << "Example: " << prog << " -v 6 -e 8 -s 1234\n";
    exit(1);
}

int main(int argc, char* argv[]) {

    int V = -1;        // Number of vertices
    int E = -1;        // Number of edges (undirected)
    unsigned int S = 0; // Seed for random generator

    // Parse command line arguments
    int opt;
    while ((opt = getopt(argc, argv, "v:e:s:")) != -1) {
        switch (opt) {
            case 'v': 
                V = atoi(optarg); // Set number of vertices
                break;
            case 'e': 
                E = atoi(optarg); // Set number of edges
                break;
            case 's': 
                S = static_cast<unsigned int>(strtoul(optarg, nullptr, 10)); // Set seed
                break;
            default: 
                usage(argv[0]); // Print usage if invalid option
        }
    }
    if (V <= 0 || E < 0) usage(argv[0]); // Check for valid input

    // Check if the number of edges is valid for a simple undirected graph
    long long maxEdges = 1LL * V * (V - 1) / 2;
    if (E > maxEdges) {
        cerr << "Error: requested edges (" << E << ") exceed maximum for simple undirected graph ("
             << maxEdges << ").\n";
        return 1;
    }

    mt19937 gen(S); // Random number generator with seed S
    
    vector<pair<int, int>> AllEdges;
    // Generate all possible edges (including self-loops)
    for(int i = 0 ; i < V; i++)
    {
        for(int j = 0; j<V; j++)
        {
            AllEdges.push_back({i,j}); // Add edge (i, j) to list
        }
    }

    // Shuffle edges randomly
    shuffle(AllEdges.begin(), AllEdges.end(), gen);

    Graph g(V); // Create an undirected graph with V vertices

    int added = 0;
    // Add edges to the graph until reaching E edges
    for (size_t i = 0; i < AllEdges.size() && added < E; ++i) {
        int u = AllEdges[i].first; 
        int v = AllEdges[i].second;
        if (g.addEdge(u, v)) // Add edge if possible (no duplicates/self-loops)
            added++;
    }
    g.printGraph(); // Print adjacency matrix

    // Check for Euler circuit and print result
    cout << "Has Euler circuit? " << (Algorithms::hasEulerCircuit(g) ? "YES" : "NO") << "\n";
    if (Algorithms::hasEulerCircuit(g)) {
        auto cyc = Algorithms::eulerCircuit(g); // Get Euler circuit
        cout << "Euler circuit: ";
        for (size_t i = 0; i < cyc.size(); ++i) {
            cout << cyc[i] << (i + 1 < cyc.size() ? " -> " : "\n"); // Print circuit
        }
    }
    return 0;
}

