// ===== AlgorithmStrategies.h =====

#pragma once
#include "AlgorithmStrategy.h"

// Strategy for Minimum Spanning Tree
class MSTStrategy : public AlgorithmStrategy {
public:
    // Run MST algorithm and return result as string
    std::string run(const Graph& g) override;
};

// Strategy for Strongly Connected Components
class SCCStrategy : public AlgorithmStrategy {
public:
    // Run SCC algorithm and return result as string
    std::string run(const Graph& g) override;
};

// Strategy for counting maximal cliques
class CountCliquesStrategy : public AlgorithmStrategy {
public:
    // Run clique counting algorithm and return result as string
    std::string run(const Graph& g) override;
};

// Strategy for finding maximum clique
class MaxCliqueStrategy : public AlgorithmStrategy {
public:
    // Run maximum clique algorithm and return result as string
    std::string run(const Graph& g) override;
};

// Strategy for checking if Euler circuit exists
class HasEulerCircuitStrategy : public AlgorithmStrategy {
public:
    // Run Euler circuit existence check and return result as string
    std::string run(const Graph& g) override;
};

// Strategy for finding Euler circuit
class EulerCircuitStrategy : public AlgorithmStrategy {
public:
    // Run Euler circuit finding algorithm and return result as string
    std::string run(const Graph& g) override;
};
