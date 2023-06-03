cmake_minimum_required(VERSION 3.24)
project(simple_mat)

add_subdirectory(src/matrix)
#add_subdirectory(src/common)
add_subdirectory(src/net)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
add_executable(main main.cpp)

