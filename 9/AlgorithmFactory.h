// ===== AlgorithmFactory.h =====
#pragma once
#include <memory>
#include <string>

class AlgorithmStrategy;

// Enum class for different algorithm types
enum class AlgorithmKind {
    MST,                // Minimum Spanning Tree
    SCC,                // Strongly Connected Components
    CountCliques,       // Count number of cliques
    MaxClique,          // Find maximum clique
    HasEulerCircuit,    // Check if Euler circuit exists
    EulerCircuit        // Find Euler circuit
};


class AlgorithmFactory {
public:
    // Create strategy by AlgorithmKind enum
    static std::unique_ptr<AlgorithmStrategy> create(AlgorithmKind kind);


    // Create strategy by algorithm name string (received from client)
    static std::unique_ptr<AlgorithmStrategy> create(const std::string& name);
};
