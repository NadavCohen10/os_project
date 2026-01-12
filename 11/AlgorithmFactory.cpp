// ===== AlgorithmFactory.cpp =====
#include "AlgorithmFactory.h"
// Include header for algorithm strategies
#include "AlgorithmStrategies.h"
// Include algorithm header for std::remove_if, std::transform
#include <algorithm>
// Include cctype for character classification functions
#include <cctype>
// Include memory header for smart pointers
#include <memory>

using namespace std;

// Helper function to normalize a string (remove spaces and convert to uppercase)
static string norm(string s){
    // Remove all whitespace characters from the string
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
    // Convert all characters to uppercase
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    // Return the normalized string
    return s;
}

// Factory method to create an algorithm strategy based on AlgorithmKind enum
std::unique_ptr<AlgorithmStrategy> AlgorithmFactory::create(AlgorithmKind kind){
    // Switch on the kind and return the corresponding strategy
    switch (kind){
        case AlgorithmKind::MST:             return make_unique<MSTStrategy>();
        case AlgorithmKind::SCC:             return make_unique<SCCStrategy>();
        case AlgorithmKind::CountCliques:    return make_unique<CountCliquesStrategy>();
        case AlgorithmKind::MaxClique:       return make_unique<MaxCliqueStrategy>();
        case AlgorithmKind::HasEulerCircuit: return make_unique<HasEulerCircuitStrategy>();
        case AlgorithmKind::EulerCircuit:    return make_unique<EulerCircuitStrategy>();
        default:                             return nullptr;
    }
}

// Factory method to create an algorithm strategy based on string name
std::unique_ptr<AlgorithmStrategy> AlgorithmFactory::create(const std::string& name){
    // Normalize the input name
    string k = norm(name);
    // Match normalized name to known algorithms and create the corresponding strategy
    if (k=="MST")               return create(AlgorithmKind::MST);
    if (k=="SCC")               return create(AlgorithmKind::SCC);
    if (k=="NUMOFCLIQUES")      return create(AlgorithmKind::CountCliques);
    if (k=="MAXCLIQUE")         return create(AlgorithmKind::MaxClique);
    if (k=="HASEULERCIRCUIT")   return create(AlgorithmKind::HasEulerCircuit);
    if (k=="EULERCIRCUIT")      return create(AlgorithmKind::EulerCircuit);
    // Return nullptr if no match found
    return nullptr;
}
