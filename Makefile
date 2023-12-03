INC_DIR = ./inc
CXX = g++
CXXFLAGS = -g -Wall -I $(INC_DIR)
DPLYFLAGS = -Wall -Werror -O2 -I $(INC_DIR)

# Source and Object files directory
SRC_DIR := src
OBJ_DIR := build

# Server files
_SERVER_O = server.o utils.o
# Client files
_CLIENT_O = client.o utils.o

SERVER_O = $(patsubst %,$(OBJ_DIR)/%,$(_SERVER_O))
CLIENT_O = $(patsubst %,$(OBJ_DIR)/%,$(_CLIENT_O))

# Build .cpp into .o (from src/ to build/)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

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
	@echo "Build complete!\n"

.PHONY: clean
clean:
	rm -f server client $(OBJ_DIR)/*.o