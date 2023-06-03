cmake_minimum_required(VERSION 3.24)
project(simple_mat)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_compile_options("-O2")

add_executable(test_matrix test.cpp utils.cpp matrix.cpp ../../include/matrix/matrix.h ../../include/matrix/point.h ../../include/matrix/utils.h)
target_compile_options(test_matrix PUBLIC -msse2)
