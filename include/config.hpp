#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "utils.hpp"

#include <CL/opencl.hpp>
#include <iostream>
#include <stdexcept>
#include <algorithm>

struct Config {
    const char* path_to_fast_ = "../fast_kernel.cl";
    const char* path_to_slow_ = "../slow_kernel.cl";  

    cl::QueueProperties q_props_ = cl::QueueProperties::Profiling; 

    Config(const char* path_to_fast = "../fast_kernel.cl", 
           const char* path_to_slow = "../slow_kernel.cl"):
        path_to_fast_{path_to_fast},
        path_to_slow_{path_to_slow} {};
};

struct option_error : public std::runtime_error {
    option_error(const char* error_name) : std::runtime_error(error_name) {}
};

#endif // CONFIG_HPP
