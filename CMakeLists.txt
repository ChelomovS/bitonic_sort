cmake_minimum_required(VERSION 3.21)
project(bitonic_sort)

find_package(GTest REQUIRED)

if (ANALYSE)
    add_definitions(-DANALYSE=1)
    message(STATUS "ANALYSE enabled")
else()
    message(STATUS "ANALYSE disabled")
endif()

if (COMPARE_CPU)
    add_definitions(-DCOMPARE_CPU=1)
    message(STATUS "COMPARE_CPU enabled")
else()
    message(STATUS "COMPARE_CPU disabled")
endif()

add_executable(${PROJECT_NAME}
    source/main.cpp
    source/config.cpp
    source/ocl_app.cpp
    source/utils.cpp
)

if(APPLE)
    set(OPENCL_CXX_HEADERS "/opt/homebrew/opt/opencl-clhpp-headers/include")
endif()

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR} ${OPENCL_CXX_HEADERS})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")
set(ASAN_FLAGS "-fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -Og -ggdb ${ASAN_FLAGS}")
target_link_libraries(${PROJECT_NAME} PRIVATE ${GTEST_BOTH_LIBRARIES})

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenCL")
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenCL)
endif()

add_subdirectory(tests)