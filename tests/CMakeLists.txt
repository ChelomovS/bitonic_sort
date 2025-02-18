cmake_minimum_required(VERSION 3.21)
project(gtest)
find_package(GTest REQUIRED)
if(NOT DEFINED INCLUDE_DIR)
    message(WARNING "INCLUDE_DIR is not defined.")
endif()

if (ANALYSE)
    add_definitions(-DANALYSE=1)
    message(STATUS "ANALYSE enabled")
else()
    message(STATUS "ANALYSE disabled")
endif()

if(APPLE)
    set(OPENCL_CXX_HEADERS "/opt/homebrew/opt/opencl-clhpp-headers/include")
endif()

aux_source_directory(unit_tests UNIT_TESTS)

add_executable(unit_tests 
               ../source/utils.cpp
               ../source/ocl_app.cpp 
               ../source/config.cpp
               ${UNIT_TESTS})
target_link_libraries(unit_tests PRIVATE ${GTEST_BOTH_LIBRARIES})
target_include_directories(unit_tests PRIVATE 
    ${INCLUDE_DIR} 
    ${GTEST_INCLUDE_DIRS}
    ${OPENCL_CXX_HEADERS} 
)

if(APPLE)
    target_link_libraries(unit_tests PRIVATE "-framework OpenCL")
else()
    target_link_libraries(unit_tests PRIVATE OpenCL)
endif()

target_compile_features(unit_tests PRIVATE cxx_std_20)

enable_testing()
add_test(NAME unit_tests COMMAND unit_tests)