INC_DIR = ./inc
CXX = g++
CXXFLAGS = -g -Wall -fsanitize=address -std=c++2a -I $(INC_DIR)
DPLYFLAGS = -Wall -Werror -O2 -std=c++2a -I $(INC_DIR)

# Source and Object files directory
SRC_DIR := src
OBJ_DIR := build

# Object files
OBJ = udp.o
# Server specific object files
_SERVER_O = $(OBJ) server.o profiles.o instances.o notifications.o
# Client specific object files
_CLIENT_O = $(OBJ) client.o

SERVER_O = $(patsubst %,$(OBJ_DIR)/%,$(_SERVER_O))
CLIENT_O = $(patsubst %,$(OBJ_DIR)/%,$(_CLIENT_O))

# Build .cpp into .o (from src/ to build/)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

# Build client and server
all: client server
	@echo "Debug build complete"

# Build server target
server: $(SERVER_O)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build client target
client: $(CLIENT_O)
	$(CXX) $(CXXFLAGS) -o $@ $^

deploy_s: $(SERVER_O)
	$(CXX) $(DPLYFLAGS) -o server $^

deploy_c: $(CLIENT_O)
	$(CXX) $(DPLYFLAGS) -o client $^

# Build client and server with optimizations
deploy: deploy_s deploy_c
	@echo "Build complete"

# Build debug client and server, and run server
test: clean all
	./server

.PHONY: clean
clean:
	rm -f server client $(OBJ_DIR)/*.o