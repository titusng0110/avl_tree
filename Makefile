# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic -O3 -Iinclude

# Directories
TEST_DIR := test
INCLUDE_DIR := include
BIN_DIR := bin

# Test Files
TEST_FILES := $(wildcard $(TEST_DIR)/*.cpp)

# Test Executables
TEST_EXECUTABLES := $(TEST_FILES:$(TEST_DIR)/%.cpp=$(BIN_DIR)/%)

# Default target
all: $(TEST_EXECUTABLES)

# Rule to build test executables
$(BIN_DIR)/%: $(TEST_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Create bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean up build artifacts
clean:
	rm -rf $(BIN_DIR)

# Phony targets
.PHONY: all clean