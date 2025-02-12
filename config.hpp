#ifndef CONFIG_HPP
#define CONFIG_HPP

#ifndef CL_HPP_TARGET_OPENCL_VERSION 
#define CL_HPP_MINIMUM_OPENCL_VERSION 120 
#define CL_HPP_TARGET_OPENCL_VERSION 120 
#endif 

#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_ENABLE_EXCEPTIONS

#include "CL/opencl.hpp"

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <string>
#include <string_view>
#include <charconv>

#include "utils.hpp"

struct Config {
    int data_size_    = 1;  
    int global_size_  = 1;  
    int local_size_   = 1;  
    const char* path_ = "kernel_for_sort.cl"; // default path

    cl::QueueProperties q_props_ = cl::QueueProperties::Profiling; 

    static Config read_config(const int argc, const char** argv);
    void dump(std::ostream& dump_stream);
};

std::ostream& operator<<(std::ostream& dump_stream, Config configurations); //FIXME - 

struct option_error : public std::runtime_error {
    option_error(const char* error_name) : std::runtime_error(error_name) {}
};

#endif // CONFIG_HPP