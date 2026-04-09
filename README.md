# simple_mat

A simple matrix library with neural network support, written in C++.

## Project Structure

```
simple_mat/
├── CMakeLists.txt  # CMake build file
├── README.md       # This documentation
├── .gitignore      # Git ignore file
├── data/           # Training and test data
│   ├── input_label_0-9_1000.xml
│   ├── input_label_1000.xml
│   └── input_label_500.xml
├── include/        # Header files
│   ├── common/     # Common utilities
│   │   ├── tinycsv.h
│   │   └── tinyxml2.h
│   ├── matrix/     # Matrix library
│   │   ├── matrix.h
│   │   ├── point.h
│   │   └── utils.h
│   └── net/        # Neural network library
│       ├── Function.h
│       └── Net.h
└── src/            # Source files
    ├── common/     # Common utilities
    │   ├── test_csv.cpp
    │   ├── test_xml.cpp
    │   ├── tinycsv.cpp
    │   └── tinyxml2.cpp
    ├── matrix/     # Matrix library
    │   ├── complex_test.cpp  # Complex test for matrix operations
    │   ├── matrix.cpp        # Matrix implementation
    │   ├── test.cpp           # Basic test for matrix operations
    │   └── utils.cpp          # Matrix utilities
    └── net/        # Neural network library
        ├── Function.cpp  # Activation functions
        ├── Net.cpp       # Neural network implementation
        └── test_net.cpp  # Test for neural network
```

## Features

### Matrix Library
- Basic matrix operations: addition, subtraction, multiplication
- Element-wise operations
- Matrix transposition
- Matrix inversion
- Triangular decomposition (upper, lower, diagonal)
- SIMD optimization for faster operations
- OpenMP parallelization for better performance

### Neural Network Library
- Feedforward neural network
- Backpropagation for training
- Support for different activation functions (sigmoid, tanh2)
- Batch training
- Loss visualization during training

## Requirements

- C++17 compiler
- CMake 3.24 or higher
- OpenMP (for parallelization)
- SIMD support (SSE2)

## Compilation

### Using CMake (Recommended)

```bash
# Create build directory
mkdir build
cd build

# Generate build files (MinGW on Windows)
cmake -G "MinGW Makefiles" ..

# Or use default generator (Unix Makefiles on Linux/Mac)
cmake ..

# Build the project
cmake --build .

# Or use make directly
mingw32-make  # Windows with MinGW
make          # Linux/Mac
```

### Build Output

After successful compilation, the following files will be generated in the `build/` directory:

- `libmatrix_lib.a` - Matrix library
- `libnet_lib.a` - Neural network library  
- `test_matrix.exe` - Matrix operations test
- `test_net.exe` - Neural network test
- `complex_test.exe` - Complex matrix operations test

## Running Tests

After compilation, all executables are located in the `build/` directory.

### Matrix Tests

```bash
cd build
./test_matrix.exe   # Basic matrix operations test (Windows)
./complex_test.exe  # Complex matrix operations test (Windows)

# On Linux/Mac
./test_matrix
./complex_test
```

### Neural Network Tests

```bash
cd build
./test_net.exe  # Neural network training and testing (Windows)

# On Linux/Mac
./test_net
```

## Usage

### Matrix Operations

```cpp
#include "matrix/matrix.h"

// Create matrices
float data1[4] = {1, 2, 3, 4};
float data2[4] = {5, 6, 7, 8};
Matrix m1(data1, 2, 2);
Matrix m2(data2, 2, 2);

// Basic operations
Matrix sum = m1.add1(m2);
Matrix diff = m1.minus1(m2);
Matrix product = m1.mul1(m2);

// Transpose
Matrix transposed = m1.transpose();

// Inversion
Matrix inverse = M_Inverse(m1);
```

### Neural Network

```cpp
#include "net/Net.h"

// Create network with 3 layers: 784 input, 100 hidden, 10 output
std::vector<int> layer_neuron_num = {784, 100, 10};
Net net;
net.initNet(layer_neuron_num);
net.initWeights(1, 0., 0.1);

// Set parameters
net.learning_rate = 0.01;
net.activation_function = "tanh2";

// Train network
net.train(input, label, 0.02, true, "data/input_label_1000.xml");

// Test network
net.test(test_input, test_label);
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License.
