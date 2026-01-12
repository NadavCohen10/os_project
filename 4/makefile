
# Set the C++ compiler
CXX = g++
# Set compiler flags: use C++11, show all warnings, enable debug info
CXXFLAGS = -std=c++11 -Wall -Wextra -g

# Name of the executable
TARGET = graph
# List of source files
SRCS = graph.cpp main.cpp Algorithms.cpp
# List of object files (replace .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

# Flags for code coverage
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage

# # Gprof flags
# Flags for gprof profiling
GPROF_FLAGS = -pg

# Arguments for valgrind
VALGRIND_ARGS = -v 1000 -e 3500 -s 1234

# Main target: build the project
all: $(TARGET)

# Link object files to create the executable with coverage flags
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) $(OBJS) -o $(TARGET)

# Compile source files into object files with coverage flags
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) -c $< -o $@

# Build and generate code coverage report with GCOV
coverage: clean all run gcov-report

# Run the executable with sample arguments
run: $(TARGET)
	./$(TARGET) -v 3 -e 3 -s 123

# Generate a simple GCOV report
gcov-report:
	gcov $(SRCS)

# Build and run with gprof profiling
gprof: clean
	$(CXX) $(CXXFLAGS) $(GPROF_FLAGS) $(SRCS) -o $(TARGET)
	./$(TARGET) -v 3 -e 3 -s 123
	mkdir -p gprof
	gprof ./graph gmon.out >./gprof/gprof_output.txt

# Run valgrind for memory checking and save report
valgrind: $(TARGET)
	mkdir -p valgrind
	valgrind --leak-check=full ./$(TARGET) $(VALGRIND_ARGS) > ./valgrind/valgrind_report.txt 2>&1

# Run callgrind for cache simulation and generate call graph
callgrind: $(TARGET)
#	valgrind --tool=callgrind --simulate-cache=yes ./$(TARGET) $(VALGRIND_ARGS)

	-valgrind --tool=callgrind --simulate-cache=yes ./$(TARGET) $(VALGRIND_ARGS)
	callgrind_annotate callgrind.out.* > callgraph.txt

# Clean all build and coverage files
clean:
	rm -f $(OBJS) $(TARGET) *.gcda *.gcno *.gcov coverage.info *.out *.txt

# Declare phony targets to avoid conflicts with files of the same name
.PHONY: all coverage run gcov-report report clean