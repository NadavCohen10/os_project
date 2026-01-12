CXX      = g++
# Set compiler flags: show all warnings, use C++17, optimize, enable pthread
CXXFLAGS = -Wall -Wextra -std=c++17 -O2 -pthread
# Set linker flags for pthread
LDFLAGS  = -pthread

# ---- Binaries ----
# Name of the regular server executable
SERVER        = server
# Name of the client executable
CLIENT        = client
# Name of the main demo executable
MAIN          = main
# Name of the pipeline server executable
PIPELINE      = pipeline_server

# ---- Sources ----
# List of common source files used by all binaries
COMMON_SRC = graph.cpp Algorithms.cpp algoMST.cpp algoSCC.cpp algoCliques.cpp
# List of source files for algorithm strategies and factory
STRAT_SRC  = AlgorithmStrategies.cpp AlgorithmFactory.cpp

# Source file for regular server
SERVER_SRC   = server.cpp
# Source file for client
CLIENT_SRC   = client.cpp
# Source file for main demo (includes strategy sources)
MAIN_SRC     = main.cpp $(STRAT_SRC)
# Source file for pipeline server (includes strategy sources)
PIPELINE_SRC = pipeline_server.cpp $(STRAT_SRC)

# ---- Objects (auto) ----
# Object files for common sources
COMMON_OBJ = $(COMMON_SRC:.cpp=.o)
# Object files for strategy sources
STRAT_OBJ  = $(STRAT_SRC:.cpp=.o)

# Object file for regular server
SERVER_OBJ   = $(SERVER_SRC:.cpp=.o)
# Object file for client
CLIENT_OBJ   = $(CLIENT_SRC:.cpp=.o)
# Object files for main demo
MAIN_OBJ     = $(MAIN_SRC:.cpp=.o)
# Object files for pipeline server
PIPELINE_OBJ = $(PIPELINE_SRC:.cpp=.o)

# ---- Default ----
# Build all binaries by default
all: $(SERVER) $(CLIENT) $(MAIN) $(PIPELINE)

# ---- Link ----
# Link object files to create the regular server executable
$(SERVER): $(COMMON_OBJ) $(STRAT_OBJ) $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Link object files to create the client executable
$(CLIENT): $(CLIENT_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Link object files to create the main demo executable
$(MAIN): $(COMMON_OBJ) $(STRAT_OBJ) $(filter %.o,$(MAIN_OBJ))
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Link object files to create the pipeline server executable
$(PIPELINE): $(COMMON_OBJ) $(STRAT_OBJ) $(filter %.o,$(PIPELINE_OBJ))
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# ---- Compile ----
# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ---- Convenience ----
# Set default port for client/server
PORT ?= 8080

# Run the regular server executable
run-server: $(SERVER)
	./$(SERVER)

# Run the pipeline server executable
run-pipeline: $(PIPELINE)
	./$(PIPELINE)

# Run the client executable with example arguments
run-client: $(CLIENT)
	./$(CLIENT) -h 127.0.0.1 -p $(PORT) -v 6 -e 8 -s 1234

# Run the client executable in auto mode
run-client-auto: $(CLIENT)
	./$(CLIENT) -h 127.0.0.1 -p $(PORT)

# Remove build artifacts
clean:
	rm -f $(SERVER) $(CLIENT) $(MAIN) $(PIPELINE) \
	      $(COMMON_OBJ) $(STRAT_OBJ) \
	      $(SERVER_OBJ) $(CLIENT_OBJ) $(MAIN_OBJ) $(PIPELINE_OBJ)

# Declare phony targets to avoid conflicts with files of the same name
.PHONY: all run-server run-pipeline run-client run-client-auto clean
