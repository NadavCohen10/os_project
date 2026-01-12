# Set the C++ compiler
CXX = g++
# Set compiler flags: show all warnings, use C++11, optimize
CXXFLAGS = -Wall -Wextra -std=c++11 -O2

# Name of the server executable
SERVER = server
# Name of the client executable
CLIENT = client

# List of source files common to both server and client
SRC_COMMON = graph.cpp Algorithms.cpp
# List of header files
HDR = graph.h Algorithms.h

# Source file for server
SRC_SERVER = server.cpp
# Source file for client
SRC_CLIENT = client.cpp

# Object files for common sources
OBJ_COMMON = $(SRC_COMMON:.cpp=.o)
# Object file for server
OBJ_SERVER = $(SRC_SERVER:.cpp=.o)
# Object file for client
OBJ_CLIENT = $(SRC_CLIENT:.cpp=.o)

# Default target: build both server and client
all: $(SERVER) $(CLIENT)

# Link object files to create the server executable
$(SERVER): $(OBJ_COMMON) $(OBJ_SERVER)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Link object files to create the client executable
$(CLIENT): $(OBJ_CLIENT)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cpp $(HDR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the server executable
run-server: $(SERVER)
	./$(SERVER)

# Run the client executable with example arguments
run-client: $(CLIENT)
	./$(CLIENT) -h 127.0.0.1 -p 8080 -v 6 -e 8 -s 1234

# Remove build artifacts
clean:
	rm -f $(SERVER) $(CLIENT) $(OBJ_COMMON) $(OBJ_SERVER) $(OBJ_CLIENT)
