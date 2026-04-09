# Makefile for simple_mat project

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -O2 -msse2 -fopenmp
INCLUDES = -I include/matrix -I include/net -I include/common

# Source files
MATRIX_SOURCES = src/matrix/matrix.cpp src/matrix/utils.cpp
NET_SOURCES = src/net/Net.cpp src/net/Function.cpp src/common/tinyxml2.cpp
TEST_MATRIX_SOURCES = src/matrix/test.cpp $(MATRIX_SOURCES)
TEST_NET_SOURCES = src/net/test_net.cpp $(NET_SOURCES) $(MATRIX_SOURCES)
COMPLEX_TEST_SOURCES = src/matrix/complex_test.cpp $(MATRIX_SOURCES)

# Executables
TEST_MATRIX = test_matrix
TEST_NET = test_net
COMPLEX_TEST = complex_test

# Default target
all: $(TEST_MATRIX) $(TEST_NET) $(COMPLEX_TEST)

# Build test_matrix
$(TEST_MATRIX): $(TEST_MATRIX_SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

# Build test_net
$(TEST_NET): $(TEST_NET_SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

# Build complex_test
$(COMPLEX_TEST): $(COMPLEX_TEST_SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

# Clean target
clean:
	rm -f $(TEST_MATRIX) $(TEST_NET) $(COMPLEX_TEST)

# Phony targets
.PHONY: all clean
