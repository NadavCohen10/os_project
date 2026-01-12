// pipeline_server.cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <random>
#include <algorithm>
#include <sstream>
#include <cstring>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>

#include "graph.h"
#include "AlgorithmFactory.h"
#include "AlgorithmStrategy.h"
#include "Algorithms.h"

#include "ThreadSafeQueue.h"
#include "ActiveObject.h"
#include "PipelineTypes.h"

#define PORT 8080
using namespace std;

// ====== Debug control ======
// Control log delay for debugging
#ifndef AO_STAGE_DELAY_MS
#define AO_STAGE_DELAY_MS 0
#endif

// Global request counter
static std::atomic<unsigned long> GREQ{0};

// Log function for pipeline stages
static void log(const char* tag, unsigned long id, const char* msg){
    std::cout << "[" << tag << "][req " << id << "] " << msg << std::endl;
}

// Delay function for simulating stage processing time
static inline void maybe_delay() {
#if AO_STAGE_DELAY_MS > 0
    std::this_thread::sleep_for(std::chrono::milliseconds(AO_STAGE_DELAY_MS));
#endif
}

// Run algorithm by name and return result string
static string run_algo_by_name(const string& name, const Graph& g) {
    auto strat = AlgorithmFactory::create(name);
    if (!strat) return "Error: unknown algorithm '" + name + "'\n";
    return strat->run(g);
}

// ===== Build random graph (simple, undirected, no self-loops) =====
// Build a random undirected graph with V vertices and E edges using seed S
static Graph build_random_graph(int V, int E, unsigned int S) {
    mt19937 gen(S);
    vector<pair<int,int>> all;
    all.reserve((size_t)V*(V-1)/2);
    for (int i=0;i<V;i++)
        for (int j=i+1;j<V;j++)
            all.push_back({i,j});
    shuffle(all.begin(), all.end(), gen);
    Graph g(V, false);
    int added=0;
    for (size_t i=0;i<all.size() && added<E; ++i) {
        if (g.addEdge(all[i].first, all[i].second, 1)) added++;
    }
    return g;
}

// Parse request string and build graph
static bool parse_request_build_graph(const string& req, Graph& outG) {
    if (req == "auto\n") {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dV(1, 10);
        int V = dV(gen);
        long long maxE = 1LL*V*(V-1)/2;
        uniform_int_distribution<long long> dE(0, maxE);
        int E = (int)dE(gen);
        unsigned int S = (unsigned int)(E*V);
        outG = build_random_graph(V,E,S);
        return true;
    } else {
        // "<V> <E> <S> [A]\n"  (A is not used here)
        istringstream iss(req);
        int V,E; unsigned int S; string A;
        if (!(iss >> V >> E >> S)) return false;
        long long maxEdges = 1LL*V*(V-1)/2;
        if (E > maxEdges || V <= 0 || E < 0) return false;
        outG = build_random_graph(V,E,S);
        return true;
    }
}

// ===== Pipeline stages =====
// Stage 1: MST algorithm
static void stage_mst(Request&& r, ThreadSafeQueue<Request>* out) {
    log("AO1-MST", r.req_id, "start");
    maybe_delay();
    r.result += run_algo_by_name("MST", r.g);
    log("AO1-MST", r.req_id, "done -> next");
    if (out) out->push(std::move(r));
}

// Stage 2: SCC algorithm
static void stage_scc(Request&& r, ThreadSafeQueue<Request>* out) {
    log("AO2-SCC", r.req_id, "start");
    maybe_delay();
    r.result += run_algo_by_name("SCC", r.g);
    log("AO2-SCC", r.req_id, "done -> next");
    if (out) out->push(std::move(r));
}

// Stage 3: Count cliques algorithm
static void stage_countCliques(Request&& r, ThreadSafeQueue<Request>* out) {
    log("AO3-COUNT", r.req_id, "start");
    maybe_delay();
    r.result += run_algo_by_name("NUMOFCLIQUES", r.g);
    log("AO3-COUNT", r.req_id, "done -> next");
    if (out) out->push(std::move(r));
}

// Stage 4: Max clique algorithm
static void stage_maxClique(Request&& r, ThreadSafeQueue<Request>* out) {
    log("AO4-MAX", r.req_id, "start");
    maybe_delay();
    r.result += run_algo_by_name("MAXCLIQUE", r.g);
    log("AO4-MAX", r.req_id, "done -> next");
    if (out) out->push(std::move(r));
}

// Stage 5: Has Euler circuit algorithm
static void stage_hasEuler(Request&& r, ThreadSafeQueue<Request>* out) {
    log("AO5-HAS-EULER", r.req_id, "start");
    maybe_delay();
    r.result += run_algo_by_name("HASEULERCIRCUIT", r.g);
    log("AO5-HAS-EULER", r.req_id, "done -> next");
    if (out) out->push(std::move(r));
}

// Stage 6: Euler circuit algorithm
static void stage_euler(Request&& r, ThreadSafeQueue<Request>* out) {
    log("AO6-EULER", r.req_id, "start");
    maybe_delay();
    r.result += run_algo_by_name("EULERCIRCUIT", r.g);
    log("AO6-EULER", r.req_id, "done -> sink");
    if (out) out->push(std::move(r));
}

// Sink stage: send reply to client and close socket
static void stage_sink(Request&& r, ThreadSafeQueue<Request>* /*out*/) {
    log("SINK", r.req_id, "sending reply to client & closing socket");
    r.result += "\n-- END-OF-PIPELINE --\n";
    ::send(r.client_fd, r.result.c_str(), r.result.size(), 0);
    ::close(r.client_fd);
}

// ===== main: acceptor + wiring =====
int main() {
    // Create server socket
    int srv = ::socket(AF_INET, SOCK_STREAM, 0);
    int opt=1;
    setsockopt(srv, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    bind(srv, (sockaddr*)&addr, sizeof(addr));
    listen(srv, 128);
    std::cout << "Pipeline server listening on " << PORT << std::endl;

    // Queues for pipeline stages
    ThreadSafeQueue<Request> q1, q2, q3, q4, q5, q6, qSink;

    // Active Objects chain for pipeline stages
    ActiveObject<Request> S1(&q1,  &q2,  stage_mst);
    ActiveObject<Request> S2(&q2,  &q3,  stage_scc);
    ActiveObject<Request> S3(&q3,  &q4,  stage_countCliques);
    ActiveObject<Request> S4(&q4,  &q5,  stage_maxClique);
    ActiveObject<Request> S5(&q5,  &q6,  stage_hasEuler);
    ActiveObject<Request> S6(&q6,  &qSink, stage_euler);
    ActiveObject<Request> Sink(&qSink, nullptr, stage_sink);

    // Start all pipeline stages
    S1.start(); S2.start(); S3.start(); S4.start(); S5.start(); S6.start(); Sink.start();

    // Acceptor loop for incoming client connections
    for (;;) {
        sockaddr_in cli{}; socklen_t len=sizeof(cli);
        int cfd = ::accept(srv, (sockaddr*)&cli, &len);
        if (cfd < 0) continue;

        char buf[1024]={0};
        ssize_t n = ::read(cfd, buf, sizeof(buf)-1);
        if (n <= 0) { ::close(cfd); continue; }
        buf[n] = '\0';
        std::string req = buf;

        Graph g(1,false);
        unsigned long id = ++GREQ;
        log("ACCEPT", id, "new connection, parsing request");

        // Parse request and build graph, handle errors
        if (!parse_request_build_graph(req, g)) {
            std::string err = "Bad request or invalid edges count.\n";
            ::send(cfd, err.c_str(), err.size(), 0);
            ::close(cfd);
            log("ACCEPT", id, "bad request, closed");
            continue;
        }
        log("ACCEPT", id, "pushed to q1");
        q1.push(Request{cfd, std::move(g), id});
    }

    // Stop and join all pipeline stages (unreachable in normal execution)
    S1.stop(); S2.stop(); S3.stop(); S4.stop(); S5.stop(); S6.stop(); Sink.stop();
    S1.join(); S2.join(); S3.join(); S4.join(); S5.join(); S6.join(); Sink.join();
    return 0;
}
