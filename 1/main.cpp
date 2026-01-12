#include <iostream>
#include "graph.h"
using namespace std;

int main() {
    Graph g(5,false); // Create an undirected graph with 5 vertices

    g.addEdge(0,1,5);
    g.addEdge(0,4);
    g.addEdge(1,2);
    g.addEdge(2,3);
    g.addEdge(3,4,5);
    g.addEdge(3,3);
    g.addEdge(4,3,-3);

    g.printGraph();

    for (int i = 0; i < 5; i++) {
        cout << "Degree of vertex " << i << " = " << g.degree(i) << endl;
    }

    return 0;
}
