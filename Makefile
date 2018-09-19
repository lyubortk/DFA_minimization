CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -Wshadow -pedantic -Werror -std=c++17 -I$(INCLUDE_DIR)

INCLUDE_DIR = include
SOURCE_DIR = src
BIN_DIR = bin
EXE = dfa_minimizer

OBJECTS = $(patsubst $(SOURCE_DIR)/%.cpp, $(BIN_DIR)/%.o, $(wildcard $(SOURCE_DIR)/*.cpp))
INCLUDES = $(wildcard $(HUF_INCL_DIR)/*.h)

all: $(EXE)

$(EXE): $(BIN_DIR) $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXE)

.SECONDEXPANSION:
$(OBJECTS): $$(patsubst $(BIN_DIR)/%.o,$(SOURCE_DIR)/%.cpp,$$@) $(INCLUDES)
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR) $(EXE)

.PHONY: clean all
