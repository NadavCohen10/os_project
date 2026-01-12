# Compiler and flags
# Use g++ as the C++ compiler
CXX = g++
# Compiler flags: warnings, C++11 standard, optimization
CXXFLAGS = -Wall -Wextra -std=c++11 -O2 

# Project settings
 # Name of the final executable
TARGET = ex1   

# Source files
SRC = main.cpp graph.cpp 
# Header file(s)          
HDR = graph.h  
 # Object files generated from source files                    
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

# Clean up build artifacts
clean:
	rm -f $(TARGET) $(OBJ)
