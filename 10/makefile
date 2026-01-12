# Set the C++ compiler
CXX      = g++
# Set compiler flags: show all warnings, use C++17, debug info, enable pthread
CXXFLAGS = -Wall -Wextra -std=c++17 -g -pthread
# Set linker flags for pthread
LDFLAGS  = -pthread

# ---- Binaries ----
# Name of the server executable
SERVER        = server
# Name of the client executable
CLIENT        = client
# Name of the main demo executable
MAIN          = main
# Name of the pipeline server executable
PIPELINE      = pipeline_server

# ---- Sources ----
# List of common source files used by all binaries
COMMON_SRC   = graph.cpp Algorithms.cpp algoMST.cpp algoSCC.cpp algoCliques.cpp
# List of source files for algorithm strategies and factory
STRAT_SRC    = AlgorithmStrategies.cpp AlgorithmFactory.cpp

# Source file for server
SERVER_SRC   = server.cpp
# Source file for client
CLIENT_SRC   = client.cpp
# Source file for main demo (includes strategy sources)
MAIN_SRC     = main.cpp $(STRAT_SRC)
# Source file for pipeline server (includes strategy sources)
PIPELINE_SRC = pipeline_server.cpp $(STRAT_SRC)

# ---- Objects ----
# Object files for common sources
COMMON_OBJ   = $(COMMON_SRC:.cpp=.o)
# Object files for strategy sources
STRAT_OBJ    = $(STRAT_SRC:.cpp=.o)
# Object file for server
SERVER_OBJ   = $(SERVER_SRC:.cpp=.o)
# Object file for client
CLIENT_OBJ   = $(CLIENT_SRC:.cpp=.o)
# Object files for main demo
MAIN_OBJ     = $(MAIN_SRC:.cpp=.o)
# Object files for pipeline server
PIPELINE_OBJ = $(PIPELINE_SRC:.cpp=.o)

# ---- Flags ----
# Flags for code coverage
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage
# Flags for gprof profiling
GPROF_FLAGS    = -pg
# Arguments for valgrind runs
VALGRIND_ARGS  = -h 127.0.0.1 -p 8080 -v 6 -e 8 -s 1234

# ---- Default ----
# Build all binaries by default
all: $(SERVER) $(CLIENT) $(MAIN) $(PIPELINE)

# ---- Link ----
# Link object files to create the server executable
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

# ---- Coverage ----
# Build, run, and generate gcov coverage report
coverage: clean all run-main gcov-report

# Run main demo for coverage
run-main: $(MAIN)
	./$(MAIN) --all

# Generate gcov report for all sources
gcov-report:
	gcov $(COMMON_SRC) $(STRAT_SRC) main.cpp

# ---- Gprof ----
# Build and run with gprof profiling, then generate report
gprof: clean
	$(CXX) $(CXXFLAGS) $(GPROF_FLAGS) $(COMMON_SRC) $(STRAT_SRC) main.cpp -o $(MAIN)
	./$(MAIN) --all
	mkdir -p gprof
	gprof ./$(MAIN) gmon.out > gprof/gprof_output.txt

# ---- Valgrind ----
# Run pipeline server with valgrind memory check
valgrind: $(PIPELINE)
	mkdir -p valgrind
	valgrind --leak-check=full ./$(PIPELINE) > valgrind/memcheck.txt 2>&1

# Run pipeline server with helgrind thread checker
helgrind: $(PIPELINE)
	mkdir -p valgrind
	valgrind --tool=helgrind ./$(PIPELINE) > valgrind/helgrind.txt 2>&1

# Run pipeline server with callgrind cache profiler
callgrind: $(PIPELINE) 
	-valgrind --tool=callgrind --simulate-cache=yes ./$(PIPELINE) 

# Run all valgrind checks and print location of reports
check-all: valgrind helgrind callgrind
	@echo "===> All Valgrind reports are in ./valgrind"

# ---- Convenience ----
# Set default port for client/server
PORT ?= 8080

# Run the server executable
run-server: $(SERVER)
	./$(SERVER)

# Run the pipeline server executable
run-pipeline: $(PIPELINE)
	./$(PIPELINE)

# Run the client executable with example arguments
run-client: $(CLIENT)
	./$(CLIENT) $(VALGRIND_ARGS)

# ---- Clean ----
# Remove all build artifacts and reports
clean:
	rm -f $(SERVER) $(CLIENT) $(MAIN) $(PIPELINE) \
	      $(COMMON_OBJ) $(STRAT_OBJ) \
	      $(SERVER_OBJ) $(CLIENT_OBJ) $(MAIN_OBJ) $(PIPELINE_OBJ) \
	      *.gcda *.gcno *.gcov gmon.out callgrind.out.*
	rm -rf valgrind gprof

# Declare phony targets to avoid conflicts with files of the same name
.PHONY: all clean coverage gprof valgrind helgrind callgrind
