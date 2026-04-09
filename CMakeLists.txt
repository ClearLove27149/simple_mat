cmake_minimum_required(VERSION 3.24)
project(simple_mat)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_compile_options("-O2")

# Include directories
include_directories(include/matrix)
include_directories(include/net)
include_directories(include/common)

# Source files
set(MATRIX_SOURCES
    src/matrix/matrix.cpp
    src/matrix/utils.cpp
)

set(NET_SOURCES
    src/net/Net.cpp
    src/net/Function.cpp
    src/common/tinyxml2.cpp
)

# Build matrix library
add_library(matrix_lib ${MATRIX_SOURCES})
target_compile_options(matrix_lib PUBLIC -msse2)

# Build net library
add_library(net_lib ${NET_SOURCES})
target_link_libraries(net_lib matrix_lib)
target_compile_options(net_lib PUBLIC -msse2)

# Build test executables
add_executable(test_matrix src/matrix/test.cpp)
target_link_libraries(test_matrix matrix_lib)

add_executable(test_net src/net/test_net.cpp)
target_link_libraries(test_net net_lib matrix_lib)

add_executable(complex_test src/matrix/complex_test.cpp)
target_link_libraries(complex_test matrix_lib)


