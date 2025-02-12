cmake_minimum_required(VERSION 3.21)
project(bitonic_sort)

add_executable(${PROJECT_NAME} 
    source/main.cpp
    source/config.cpp
    source/ocl_app.cpp
    source/utils.cpp
)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# set(ASAN_FLAGS "-fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -Og -ggdb ${ASAN_FLAGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Wshadow -Wunused")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE "OpenCL")
