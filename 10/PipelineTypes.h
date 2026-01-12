// PipelineTypes.h
#pragma once
// Include string header for std::string
#include <string>
// Include graph class header
#include "graph.h"

// Structure to represent a client request in the pipeline
struct Request {
    int client_fd;       // Socket file descriptor for client connection
    Graph g;             // Graph object for processing
    std::string result;  // Accumulated results for output
    unsigned long req_id;// Unique request identifier

    // Constructor: initialize with lvalue graph
    Request(int fd, const Graph& graph, unsigned long id)
        : client_fd(fd), g(graph), req_id(id) {}

    // Constructor: initialize with rvalue graph (move)
    Request(int fd, Graph&& graph, unsigned long id)
        : client_fd(fd), g(std::move(graph)), req_id(id) {}
};
