#include "ocl_app.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

cl::Event OclApp::bitonic_sort(int* data, size_t data_size) {
    size_t array_buf_size = data_size * sizeof(int);
    cl::Buffer array_buffer(context_, CL_MEM_READ_WRITE, array_buf_size);
    cl::copy(queue_, data, data + data_size, array_buffer);
    cl::Program program(context_, kernel_, true);
    bitonic_sort_t bitonic_sort(program, "bitonic_sort");

    cl::Event last_event;

    cl_ulong pure_time  = 0;
    cl_ulong start_time = 0;
    cl_ulong end_time   = 0;

    int stages = std::log2(data_size);

    for (int stage = 1; stage <= stages; ++stage) {
        for (int phase_of_stage = 1; phase_of_stage <= stage; ++phase_of_stage) {
            cl::NDRange global(configurations_.global_size_);
            cl::NDRange local(configurations_.local_size_); 
            cl::EnqueueArgs Args(queue_, global, local);

            cl::Event event = bitonic_sort(Args, array_buffer, stage, phase_of_stage);

            event.wait();

            start_time = event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
            end_time = event.getProfilingInfo<CL_PROFILING_COMMAND_END>();
            pure_time += (end_time - start_time); // ns time 

            last_event = event;
        }
    }

    dbgs << ">>> GPU pure time measured: " << (pure_time / 1'000'000) << " ms <<<" << std::endl; // ms time 
 
    cl::copy(queue_, array_buffer, data, data + data_size);

    return last_event;
}

std::string OclApp::read_kernel_file(const char* path) {
    std::string kernel_code;
    std::ifstream shader_file;
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    shader_file.open(path);
    std::stringstream shader_stream;
    shader_stream << shader_file.rdbuf();
    shader_file.close();
    kernel_code = shader_stream.str();
    return kernel_code;
}

cl::Platform OclApp::select_platform() {
    cl::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    for (auto platform : platforms) {
        cl_uint number_devices = 0;
        ::clGetDeviceIDs(platform(), CL_DEVICE_TYPE_GPU, 0, NULL, &number_devices);
        if (number_devices > 0)
            return cl::Platform(platform);
    }
    throw std::runtime_error("Platform is not selected!");
}

cl::Context OclApp::get_gpu_context(cl_platform_id platform_id) {
    cl_context_properties properties[] = {
        CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(platform_id), 0
    };

    return cl::Context(CL_DEVICE_TYPE_GPU, properties);
}
