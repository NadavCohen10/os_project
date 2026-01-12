// ===== AlgorithmStrategy.h =====

#pragma once
#include <string>
class Graph;

class AlgorithmStrategy {
public:
    virtual ~AlgorithmStrategy() = default;
    
    // Run the algorithm and return the result as a string ready to be sent to the client
    virtual std::string run(const Graph& g) = 0;
};
