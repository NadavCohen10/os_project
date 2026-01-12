#include <iostream>
// Include socket library
#include <sys/socket.h>
// Include internet address family definitions
#include <netinet/in.h>
// Include POSIX close function
#include <unistd.h>
// Include graph class header
#include "graph.h"
// Include algorithms header
#include "Algorithms.h"
// Include random number generation
#include <random>
// Include algorithms for shuffling
#include <algorithm>

// Define server port
#define PORT 8080
using namespace std;

int main()
{
    // Server and client socket file descriptors
    int server_fd, new_socket;
    // Structure for server address
    struct sockaddr_in address;
    // Option for socket reuse
    int opt = 1;
    // Length of address structure
    socklen_t addrlen = sizeof(address);

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Set socket options to reuse address and port
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    // Set address family to IPv4
    address.sin_family = AF_INET;
    // Accept connections from any IP address
    address.sin_addr.s_addr = INADDR_ANY;
    // Set port number
    address.sin_port = htons(PORT);

    // Bind socket to address and port
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    // Listen for incoming connections, max 3 in queue
    listen(server_fd, 3);

    // Print server listening message
    std::cout << "Server listening on port " << PORT << std::endl;

    // Main server loop
    while (true)
    {
        // Accept a new connection
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        std::cout << " find new connection" << std::endl;

        // Buffer for incoming data
        char buffer[1024] = {0};
        // Read data from client
        ssize_t n = read(new_socket, buffer, sizeof(buffer) - 1);
        if (n < 0) {
            // Print error if read fails
            perror("read");
            // Close client socket
            close(new_socket);
            // Continue to next connection
            continue;
        }
        // Null-terminate the received string
        buffer[n] = '\0';

        // Variables for vertices, edges, and seed
        int V, E, S;
        // Parse input from client
        sscanf(buffer, "%d %d %d", &V, &E, &S);
        cout<<E<<endl;

        // Response string to send to client
        std::string response;

        // Calculate maximum number of edges for simple undirected graph
        long long maxEdges = 1LL * V * (V - 1) / 2;
        std::cout << maxEdges << endl;
        // Check if requested edges exceed maximum
        if (E > maxEdges)
        {
            response = "Error: requested edges (" + to_string(E) +
           ") exceed maximum for simple undirected graph (" +
           to_string(maxEdges) + ").\n";

            cout<<response << endl;
        }

        // If response is empty, proceed to generate graph
        if (response.empty())
        {
            // Create random number generator with seed S
            mt19937 gen(S);

            // Vector to store all possible edges
            vector<pair<int, int>> AllEdges;
            // Generate all possible edges (including self-loops)
            for (int i = 0; i < V; i++)
            {
                for (int j = 0; j < V; j++)
                {
                    AllEdges.push_back({i, j}); // Add edge (i, j) to list
                }
            }

            // Shuffle edges randomly
            shuffle(AllEdges.begin(), AllEdges.end(), gen);

            // Create an undirected graph with V vertices
            Graph g(V);

            int added = 0;
            // Add edges to the graph until reaching E edges
            for (size_t i = 0; i < AllEdges.size() && added < E; ++i)
            {
                int u = AllEdges[i].first;
                int v = AllEdges[i].second;
                // Add edge if possible (no duplicates/self-loops)
                if (g.addEdge(u, v))
                    added++;
            }
            // Print adjacency matrix
            g.printGraph();

            // Check if graph has Euler circuit
            bool has = Algorithms::hasEulerCircuit(g);

            if (has)
            {
                // Get Euler circuit path
                auto path = Algorithms::eulerCircuit(g);
                response = "YES ";
                // Format path as string
                for (size_t i = 0; i < path.size(); i++)
                {
                    response += std::to_string(path[i]);
                    if (i + 1 < path.size())
                        response += "->";
                }
            }
            else
            {
                // No Euler circuit found
                response = "NO";
            }
        }
        // Send response to client
        send(new_socket, response.c_str(), response.size(), 0);
        // Close client socket
        close(new_socket);
    }

    // Exit
    return 0;
}
