#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "utils.hpp"

#include <CL/opencl.hpp>
#include <iostream>
#include <stdexcept>

struct Config {
    int data_size_    = 1; 
    int global_size_  = 1;  
    int local_size_   = 1; 
    const char* path_ = "../kernel_for_sort.cl"; 
    cl::QueueProperties q_props_ = cl::QueueProperties::Profiling; 

    Config(int data_size, const char* path = "../kernel_for_sort.cl"): 
        data_size_{data_size}, 
        global_size_{data_size}, 
        path_{path} {}

    void dump(std::ostream& dump_stream);
};

struct option_error : public std::runtime_error {
    option_error(const char* error_name) : std::runtime_error(error_name) {}
};

#endif // CONFIG_HPP
