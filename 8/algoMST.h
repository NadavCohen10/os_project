// ===== algoMST.h =====
#pragma once
#include <vector>
using namespace std;
class Graph;

class algoMST
{
public:
    // 1) MST total weight (for undirected, connected graph). If not connected, returns -1.
    static long long mstWeight(const Graph &g);
};