# Set the C++ compiler
CXX      = g++
# Set compiler flags: show all warnings, use C++14, optimize, enable pthread
CXXFLAGS = -Wall -Wextra -std=c++14 -O2 -pthread
# Set linker flags for pthread
LDFLAGS  = -pthread

# ---- Binaries ----
# Name of the server executable
SERVER   = server
# Name of the client executable
CLIENT   = client
# Name of the main demo executable
MAIN     = main

# ---- Sources ----
# List of common source files used by all binaries
COMMON_SRC = graph.cpp Algorithms.cpp algoMST.cpp algoSCC.cpp algoCliques.cpp
# List of source files for algorithm strategies and factory
STRAT_SRC  = AlgorithmStrategies.cpp AlgorithmFactory.cpp

# Source file for server
SERVER_SRC = server.cpp
# Source file for client
CLIENT_SRC = client.cpp
# Source file for main demo (includes strategy sources)
MAIN_SRC   = main.cpp $(STRAT_SRC)

# ---- Objects (auto) ----
# Object files for common sources
COMMON_OBJ = $(COMMON_SRC:.cpp=.o)
# Object files for strategy sources
STRAT_OBJ  = $(STRAT_SRC:.cpp=.o)

# Object file for server
SERVER_OBJ = $(SERVER_SRC:.cpp=.o)
# Object file for client
CLIENT_OBJ = $(CLIENT_SRC:.cpp=.o)
# Object files for main demo
MAIN_OBJ   = $(MAIN_SRC:.cpp=.o)

# ---- Default ----
# Build all binaries by default
all: $(SERVER) $(CLIENT) $(MAIN)

# ---- Link ----
# Link object files to create the server executable (includes strategy and factory)
$(SERVER): $(COMMON_OBJ) $(STRAT_OBJ) $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Link object files to create the client executable
$(CLIENT): $(CLIENT_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Link object files to create the main demo executable
$(MAIN): $(COMMON_OBJ) $(STRAT_OBJ) $(filter %.o,$(MAIN_OBJ))
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# ---- Compile ----
# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ---- Convenience ----
# Set default port for client/server
PORT ?= 8080

# Run the server executable
run-server: $(SERVER)
	./$(SERVER)

# Run the client executable with example arguments
run-client: $(CLIENT)
	./$(CLIENT) -h 127.0.0.1 -p $(PORT) -v 6 -e 8 -s 1234

# Run the client executable in auto mode
run-client-auto: $(CLIENT)
	./$(CLIENT) -h 127.0.0.1 -p $(PORT)

# Remove build artifacts
clean:
	rm -f $(SERVER) $(CLIENT) $(MAIN) \
	      $(COMMON_OBJ) $(STRAT_OBJ) \
	      $(SERVER_OBJ) $(CLIENT_OBJ) $(MAIN_OBJ)

# Declare phony targets to avoid conflicts with files of the same name
.PHONY: all run-server run-client run-client-auto clean
