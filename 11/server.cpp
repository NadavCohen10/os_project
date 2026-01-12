// server.cpp — Leader–Follower multithreaded server
#include <iostream>
// Include vector header for std::vector
#include <vector>
// Include string header for std::string
#include <string>
// Include stringstream for parsing input
#include <sstream>
// Include random number generation
#include <random>
// Include algorithm header for std::shuffle
#include <algorithm>
// Include thread support
#include <thread>
// Include mutex for thread synchronization
#include <mutex>
// Include condition variable for thread coordination
#include <condition_variable>
// Include atomic for atomic flags
#include <atomic>
// Include signal handling
#include <csignal>
// Include socket library
#include <sys/socket.h>
// Include internet address family definitions
#include <netinet/in.h>
// Include POSIX close function
#include <unistd.h>
// Include C string manipulation functions
#include <cstring>
// Include graph class header
#include "graph.h"
// Include algorithms header
#include "Algorithms.h"
// Include algorithm factory header
#include "AlgorithmFactory.h"
// Include algorithm strategy interface header
#include "AlgorithmStrategy.h"

#define PORT 8080
using namespace std;

// List of algorithm names to run for each client
static vector<string> algoNames = {
    "MST", "SCC", "NUMOFCLIQUES", "MAXCLIQUE", "HASEULERCIRCUIT", "EULERCIRCUIT"
};

// Helper function to run algorithm by name and return result as string
static string run_algo_by_name(const string& name, const Graph& g) {
    auto strat = AlgorithmFactory::create(name);
    if (!strat) return "Error: unknown algorithm '" + name + "'\n";
    return strat->run(g);
}

// Global server socket and control flags
static int g_server_fd = -1;
static sockaddr_in g_address{};
static socklen_t g_addrlen = sizeof(g_address);
static std::atomic<bool> g_running{true};

// Leader–Follower baton synchronization
static std::mutex g_mtx;
static std::condition_variable g_cv;
static bool g_leader_taken = false; // true if a leader is currently handling accept

// Graceful shutdown handler for SIGINT (Ctrl+C)
static void handle_sigint(int) {
    g_running = false;
    // Close listening socket to wake accept()
    if (g_server_fd >= 0) ::shutdown(g_server_fd, SHUT_RDWR);
    // Notify threads to avoid waiting forever
    {
        std::lock_guard<std::mutex> lk(g_mtx);
        g_cv.notify_all();
    }
}

// Per-client handler: processes a single client connection
static void handle_client(int new_socket) {
    // Buffer for incoming data
    char buffer[1024] = {0};
    // Read data from client
    ssize_t n = ::read(new_socket, buffer, sizeof(buffer) - 1);
    if (n < 0) {
        perror("read");
        ::close(new_socket);
        return;
    }
    // Null-terminate the received string
    buffer[n] = '\0';

    // Variables for vertices, edges, seed, and algorithm name
    int V = 0, E = 0, S = 0;
    std::string A;

    // Check for auto mode or parse input
    if (std::strcmp(buffer, "auto\n") == 0) {
        // Auto mode: generate random V, E, S
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distV(1, 10);
        V = distV(gen);

        long long maxVal = 1LL * V * (V - 1) / 2;
        std::uniform_int_distribution<long long> distE(0, maxVal);
        E = static_cast<int>(distE(gen));
        S = E * V; // Seed derived from E and V
    } else {
        // Parse input from client
        std::istringstream iss(buffer);
        iss >> V >> E >> S >> A;
    }

    std::string response;

    // Check if requested edges exceed maximum for simple undirected graph
    long long maxEdges = 1LL * V * (V - 1) / 2;
    if (E > maxEdges) {
        response = "Error: requested edges (" + std::to_string(E) +
                   ") exceed maximum for simple undirected graph (" +
                   std::to_string(maxEdges) + ").\n";
        ::send(new_socket, response.c_str(), response.size(), 0);
        ::close(new_socket);
        return;
    }

    // Build random graph
    std::mt19937 gen(static_cast<unsigned int>(S));
    std::vector<std::pair<int,int>> AllEdges;
    AllEdges.reserve((size_t)V * (size_t)V);
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            AllEdges.push_back({i, j});

    std::shuffle(AllEdges.begin(), AllEdges.end(), gen);

    // Create an undirected graph with V vertices
    Graph g(V);
    int added = 0;
    // Add edges to the graph until reaching E edges
    for (size_t i = 0; i < AllEdges.size() && added < E; ++i) {
        int u = AllEdges[i].first;
        int v = AllEdges[i].second;
        // Add edge if possible (no duplicates/self-loops)
        if (g.addEdge(u, v))
            ++added;
    }

    // Run all algorithms and append results to response
    for (const auto& name : algoNames)
        response += run_algo_by_name(name, g);

    // Send response to client and close connection
    ::send(new_socket, response.c_str(), response.size(), 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    ::close(new_socket);
}

// Worker thread loop for Leader–Follower pattern
static void worker_loop() {
    // Each thread loops: become leader, accept client, release leadership, process client, repeat
    while (g_running) {
        // Become leader (wait if another leader exists)
        {
            std::unique_lock<std::mutex> lk(g_mtx);
            g_cv.wait(lk, []{
                return !g_leader_taken || !g_running.load();
            });
            if (!g_running) return;
            g_leader_taken = true; // This thread is now the leader
        }

        // Perform accept() as leader
        int new_socket = ::accept(g_server_fd, reinterpret_cast<sockaddr*>(&g_address), &g_addrlen);

        // Promote another follower to leader before processing client
        {
            std::lock_guard<std::mutex> lk(g_mtx);
            g_leader_taken = false;     // Release leadership
            g_cv.notify_one();          // Wake up another thread to become leader
        }

        if (new_socket < 0) {
            // Accept failed (e.g., during shutdown), check if shutting down
            if (!g_running) return;
            perror("accept");
            // Try again in loop
            continue;
        }

        // Process client as follower
        handle_client(new_socket);
    }
}

int main() {
    // Register SIGINT handler for graceful shutdown
    std::signal(SIGINT, handle_sigint);

    // Create listening socket
    g_server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (g_server_fd < 0) { perror("socket"); return 1; }

    int opt = 1;
    ::setsockopt(g_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    g_address.sin_family = AF_INET;
    g_address.sin_addr.s_addr = INADDR_ANY;
    g_address.sin_port = htons(PORT);

    if (::bind(g_server_fd, reinterpret_cast<sockaddr*>(&g_address), sizeof(g_address)) < 0) {
        perror("bind"); ::close(g_server_fd); return 1;
    }
    if (::listen(g_server_fd, 128) < 0) {
        perror("listen"); ::close(g_server_fd); return 1;
    }

    std::cout << "LF Server listening on port " << PORT << std::endl;

    // Create thread pool for worker threads
    const unsigned N = std::max(4u, std::thread::hardware_concurrency());
    std::vector<std::thread> pool;
    pool.reserve(N);
    {
        std::lock_guard<std::mutex> lk(g_mtx);
        g_leader_taken = false; // No leader at start
    }
    for (unsigned i = 0; i < N; ++i)
        pool.emplace_back(worker_loop);

    // Wait for shutdown signal and join all threads
    for (auto& t : pool) t.join();

    // Close server socket
    ::close(g_server_fd);
    return 0;
}
