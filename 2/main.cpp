#include <iostream>
#include "graph.h"
#include "Algorithms.h"
using namespace std;

int main() {
    Graph g(14);
    g.addEdge(0, 13);
    g.addEdge(13, 1);
    g.addEdge(1, 12);
    g.addEdge(12, 2);
    g.addEdge(2, 11);
    g.addEdge(11, 3);
    g.addEdge(3, 10);
    g.addEdge(10, 4);
    g.addEdge(4, 9);
    g.addEdge(9, 5);
    g.addEdge(5, 8);
    g.addEdge(8, 6);
    g.addEdge(6, 7);
    g.addEdge(7, 0); // Closing the cycle
   

    g.printGraph();

    cout << "Has Euler circuit? " << (Algorithms::hasEulerCircuit(g) ? "YES" : "NO") << "\n";
    if (Algorithms::hasEulerCircuit(g)) {
        auto cyc = Algorithms::eulerCircuit(g);
        cout << "Euler circuit: ";
        for (size_t i = 0; i < cyc.size(); ++i) {
            cout << cyc[i] << (i + 1 < cyc.size() ? " -> " : "\n");
        }
    }
    return 0;
}
