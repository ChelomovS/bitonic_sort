#include <iostream>
#include <chrono>

#ifndef CL_HPP_TARGET_OPENCL_VERSION 
#define CL_HPP_MINIMUM_OPENCL_VERSION 120 
#define CL_HPP_TARGET_OPENCL_VERSION 120 
#endif 

#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_ENABLE_EXCEPTIONS

#include "CL/opencl.hpp"

#include "utils.hpp"
#include "config.hpp"
#include "ocl_app.hpp"

#ifndef COMPARE_CPU
#define COMPARE_CPU 1
#endif

int main(const int argc, const char** argv) try {
    std::chrono::high_resolution_clock::time_point start_time, final_time;
    cl_long wall_time;

    Config configurations = Config::read_config(argc, argv);
    dbgs << "read configs: " << configurations << std::endl;  
    OclApp application(configurations);

    cl::vector<int> data(configurations.data_size_);
    random_init(data.begin(), data.end(), 0, 100000);

    start_time = std::chrono::high_resolution_clock::now();
    
    cl::Event event = application.bitonic_sort(data.data(), data.size());
    final_time = std::chrono::high_resolution_clock::now();

    wall_time = std::chrono::duration_cast<std::chrono::milliseconds>(final_time - start_time).count();
    dbgs << "GPU wall time measured: " << wall_time << " ms" << std::endl;

    if (check_sort(data)) {
        dbgs << "array is sorted" << std::endl;
    } else {
        dbgs << "array is not sorted" << std::endl;
    }

#if COMPARE_CPU 
    std::vector<int> array_CPU(configurations.data_size_);
    random_init(array_CPU.begin(), array_CPU.end(), 0, 100000);
    start_time = std::chrono::high_resolution_clock::now();
    std::sort(array_CPU.begin(), array_CPU.end());
    final_time = std::chrono::high_resolution_clock::now();
    wall_time = std::chrono::duration_cast<std::chrono::milliseconds>(final_time - start_time).count();
    std::cout << "CPU time measured: " << wall_time << " ms" << std::endl;
#endif 

    dbgs << "All checks passed" << std::endl;
} catch (cl::BuildError &err) {
    std::cerr << "OCL BUILD ERROR: " << err.err() << ":" << err.what()
              << std::endl;
    std::cerr << "-- Log --\n";
    for (auto e : err.getBuildLog())
        std::cerr << e.second;
    std::cerr << "-- End log --\n";
    return -1;
} catch (cl::Error &err) {
    std::cerr << "OCL ERROR: " << err.err() << ":" << err.what() << std::endl;
    return -1;
} catch (option_error &err) {
    std::cerr << "INVALID OPTION: " << err.what() << std::endl;
    return -1;
} catch (std::runtime_error &err) {
    std::cerr << "RUNTIME ERROR: " << err.what() << std::endl;
    return -1;
} catch (...) {
    std::cerr << "UNKNOWN ERROR\n";
    return -1;
}
