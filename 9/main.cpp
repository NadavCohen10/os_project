#include <iostream>
// Include string header for std::string
#include <string>
// Include vector header for std::vector
#include <vector>
// Include graph class header
#include "graph.h"
// Include algorithm factory header
#include "AlgorithmFactory.h"
// Include algorithm strategy interface header
#include "AlgorithmStrategy.h"

using namespace std;

// Print usage instructions and exit
static void usage(const char* prog) {
    cerr << "Usage:\n"
         << "  " << prog << " --algo <MST|SCC|Cliques|MaxClique>\n"
         << "  " << prog << " --all\n"
         << "Optional graph args:\n"
         << "  --undirected (default) | --directed\n"
         << "  --V <num_vertices>\n"
         << "  --E <num_edges>       (ignored if you build graph manually)\n"
         << "\nNote: This demo builds a tiny fixed graph by code.\n";
    exit(1);
}

// Build a demo graph for testing algorithms
static Graph build_demo_graph(bool directed=false) {
    Graph g(6, directed);
    // Add edges to the graph with weight 1
    g.addEdge(0,1,1);
    g.addEdge(0,2,1);
    g.addEdge(1,2,1);
    g.addEdge(1,3,1);
    g.addEdge(1,4,1);
    g.addEdge(3,4,1);
    g.addEdge(2,5,1);
    return g;
}

// Run the selected algorithm by name and return the result as string
static string run_algo_by_name(const string& name, const Graph& g) {
    auto strat = AlgorithmFactory::create(name);
    if (!strat) return "Error: unknown algorithm '" + name + "'";
    return strat->run(g);
}

int main(int argc, char** argv) {
    // Check for minimum number of arguments
    if (argc < 2) usage(argv[0]);

    // Initialize flags and variables
    bool directed = false;
    bool runAll = false;
    string algoName;

    // Basic argument parsing loop
    for (int i=1; i<argc; ++i) {
        string a = argv[i];
        // Set directed flag if argument is --directed
        if (a == "--directed") directed = true;
        // Set undirected flag if argument is --undirected
        else if (a == "--undirected") directed = false;
        // Set runAll flag if argument is --all
        else if (a == "--all") runAll = true;
        // Set algorithm name if argument is --algo
        else if (a == "--algo" && i+1 < argc) { algoName = argv[++i]; }
    }
    // If no algorithm specified and not running all, show usage
    if (!runAll && algoName.empty()) usage(argv[0]);

    // Build the demo graph
    Graph g = build_demo_graph(directed);

    // Print the graph
    cout << "Graph:\n";
    g.printGraph();
    cout << "\n=== RESULTS ===\n";

    // Run all algorithms if runAll is true
    if (runAll) {
        vector<string> names = {"MST", "SCC", "Num Of Cliques", "MaxClique"};
        for (const auto& n : names) {
            cout << "[" << n << "]\n";
            cout << run_algo_by_name(n, g) << "\n\n";
        }
    } else {
        // Run the selected algorithm
        cout << "[" << algoName << "]\n";
        cout << run_algo_by_name(algoName, g) << "\n";
    }
    // Return success
    return 0;
}
