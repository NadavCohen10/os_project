// client.cpp
// Include standard input/output stream library
#include <iostream>
// Include string library
#include <string>
// Include C string manipulation functions
#include <cstring>
// Include C standard library functions
#include <cstdlib>
// Include POSIX API for command-line argument parsing
#include <unistd.h>
// Include definitions for internet operations
#include <arpa/inet.h>
// Include definitions for sockets
#include <sys/socket.h>
// Include definitions for internet address family
#include <netinet/in.h>

// Print usage instructions and exit
static void usage(const char* prog) {
    std::cerr << "Usage: " << prog
              << " [-h <host=127.0.0.1>] [-p <port=8080>] -v <vertices> -e <edges> -s <seed>\n"
              << "Example: " << prog << " -h 127.0.0.1 -p 8080 -v 6 -e 8 -s 1234\n";
    std::exit(1);
}

int main(int argc, char* argv[]) {
    // Default values for host, port, vertices, edges, and seed
    std::string host = "127.0.0.1";
    int port = 8080;
    int V = -1, E = -1;
    unsigned int S = 0;

    // Parse command-line arguments
    int opt;
    while ((opt = getopt(argc, argv, "h:p:v:e:s:")) != -1) {
        switch (opt) {
            case 'h': host = optarg; break;
            case 'p': port = std::atoi(optarg); break;
            case 'v': V = std::atoi(optarg); break;
            case 'e': E = std::atoi(optarg); break;
            case 's': S = static_cast<unsigned int>(std::strtoul(optarg, nullptr, 10)); break;
            default: usage(argv[0]);
        }
    }
    // Check for valid vertices and edges values
    if (V <= 0 || E < 0) usage(argv[0]);

    // Create a TCP socket
    int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    // Build server address structure
    sockaddr_in serv{};
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    if (::inet_pton(AF_INET, host.c_str(), &serv.sin_addr) <= 0) {
        std::cerr << "Invalid address: " << host << "\n";
        ::close(sockfd);
        return 1;
    }

    // Connect to the server
    if (::connect(sockfd, reinterpret_cast<sockaddr*>(&serv), sizeof(serv)) < 0) {
        perror("connect");
        ::close(sockfd);
        return 1;
    }

    // Send the request to the server in the format "V E S\n"
    std::string req = std::to_string(V) + " " + std::to_string(E) + " " + std::to_string(S) + "\n";
    ssize_t sent = ::send(sockfd, req.data(), req.size(), 0);
    if (sent < 0) {
        perror("send");
        ::close(sockfd);
        return 1;
    }

    // Receive the reply from the server
    std::string reply;
    char buf[1024];
    for (;;) {
        ssize_t n = ::recv(sockfd, buf, sizeof(buf)-1, 0);
        if (n < 0) {
            perror("recv");
            ::close(sockfd);
            return 1;
        }
        // If connection closed, exit loop
        if (n == 0) break;
        buf[n] = '\0';
        reply += buf;
    }

    // Close the socket
    ::close(sockfd);

    // Print the reply (e.g., "NO" or "YES 0->1->2->0")
    std::cout << reply << std::endl;
    return 0;
}
