cmake_minimum_required(VERSION 3.24)
project(simple_mat)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_compile_options("-O2")

include_directories(../../include/matrix)
include_directories(../../include/net)


add_executable(test_net test_net.cpp Net.cpp Function.cpp ../matrix/utils.cpp ../matrix/matrix.cpp ../common/tinyxml2.cpp
        ../../include/net/Net.h ../../include/net/Function.h ../../include/matrix/matrix.h ../../include/matrix/point.h
        ../../include/matrix/utils.h ../../include/common/tinyxml2.h)
target_compile_options(test_net PUBLIC -msse2)
