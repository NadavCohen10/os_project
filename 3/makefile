# Set the C++ compiler
CXX = g++
# Set compiler flags: show all warnings, use C++11, optimize
CXXFLAGS = -Wall -Wextra -std=c++11 -O2

# Name of the executable to build
TARGET = ex3
# List of source files
SRC = main.cpp graph.cpp Algorithms.cpp
# List of header files
HDR = graph.h algorithms.h
# List of object files (replace .cpp with .o)
OBJ = $(SRC:.cpp=.o)

# Default target: build the executable
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

# Compile source files into object files
%.o: %.cpp $(HDR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the executable
run: $(TARGET)
	./$(TARGET)

# Remove build artifacts
clean:
	rm -f $(TARGET) $(OBJ)
