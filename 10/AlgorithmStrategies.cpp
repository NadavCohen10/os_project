// ===== AlgorithmStrategies.cpp =====
#include "AlgorithmStrategies.h"
// Include algorithms header for graph algorithms
#include "Algorithms.h"
// Include graph header for graph operations
#include "graph.h"
// Include string header for std::string
#include <string>
// Include stringstream for output formatting
#include <sstream>
// Include vector header for std::vector
#include <vector>
// Include algorithm header for std::max_element
#include <algorithm>
// Include numeric header for std::iota
#include <numeric>
// Include MST algorithm implementation
#include "algoMST.h"
// Include SCC algorithm implementation
#include "algoSCC.h"
// Include cliques algorithm implementation
#include "algoCliques.h"

using std::string; using std::ostringstream;

// Strategy for Minimum Spanning Tree
std::string MSTStrategy::run(const Graph& g) {
    // Compute MST total weight
    long long w = algoMST::mstWeight(g);
    // If graph is not connected, return message
    if (w < 0) return "MST: graph is not connected (no spanning tree).";
    // Return MST total weight
    return "MST total weight = " + std::to_string(w);
}

// Strategy for Strongly Connected Components
std::string SCCStrategy::run(const Graph& g) {
    // Compute SCC components
    auto comp = algoSCC::scc(g);
    // Calculate number of components
    int num = comp.empty()? 0 : (*std::max_element(comp.begin(), comp.end()) + 1);
    ostringstream out;
    // Output number of components
    out << "SCC components = " << num << "\n";
    // Output mapping of nodes to components
    for (size_t i=0;i<comp.size();++i) out << "  node " << i << " -> comp " << comp[i] << "\n";
    return out.str();
}

// Strategy for counting maximal cliques
std::string CountCliquesStrategy::run(const Graph& g) {
    // Count maximal cliques using Bron–Kerbosch algorithm
    long long c = algoCliques::countCliques(g);
    // Return number of maximal cliques
    return "Number of maximal cliques = " + std::to_string(c);
}

// Strategy for finding maximum clique
std::string MaxCliqueStrategy::run(const Graph& g) {
    // Find largest clique in the graph
    auto best = algoCliques::maxClique(g);
    ostringstream out;
    // Output size of maximum clique
    out << "Maximum clique size = " << best.size() << "\n{ ";
    // Output vertices in the maximum clique
    for (size_t i=0;i<best.size();++i) out << best[i] << (i+1<best.size()? ", ":" ");
    out << "}";
    return out.str();
}

// Strategy for checking if Euler circuit exists
std::string HasEulerCircuitStrategy::run(const Graph& g) {
    // Check if graph has Euler circuit
    bool hasEulerCircuit = Algorithms::hasEulerCircuit(g);
    // Return result message
    if(hasEulerCircuit)
        return "there is Euler Circuit in the graph";
    return "there is not Euler Circuit in the graph";
}

// Strategy for finding Euler circuit
std::string EulerCircuitStrategy::run(const Graph& g) {
    std::ostringstream out;
    // Check if graph has Euler circuit
    if (!Algorithms::hasEulerCircuit(g)) {
        out << "Euler circuit: NONE";
        return out.str();
    }
    // Get Euler circuit path
    auto path = Algorithms::eulerCircuit(g);
    // Output length of Euler circuit
    out << "Euler circuit length = " << path.size() << "\n";
    out << "{ ";
    // Output vertices in Euler circuit path
    for (size_t i = 0; i < path.size(); ++i) {
        out << path[i] << (i + 1 < path.size() ? " -> " : " ");
    }
    out << "}";
    return out.str();
}

