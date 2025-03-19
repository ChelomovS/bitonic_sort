#include "ocl_app.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include <algorithm>

long int OclApp::bitonic_sort(int* data, size_t data_size) {
    if (data_size == 1) 
        return 0;
    
    size_t array_buf_size = data_size * sizeof(int);
    cl::vector<cl::Event> kernel_events;
    cl::Event copy_event;

    cl::Buffer array_buffer(context_, CL_MEM_READ_WRITE, array_buf_size);
    
    auto t1 = std::chrono::high_resolution_clock::now();
    cl::copy(queue_, data, data + data_size, array_buffer);
    auto t2 = std::chrono::high_resolution_clock::now();

    data_transfer_time_ += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

    cl::Program program_1(context_, sort_fast, true);
    cl::Program program_2(context_, sort_default, true);
    
    bitonic_sort_fast_t bitonic_sort_fast(program_1, "bitonic_sort_local");

    cl::vector<cl::Event> stage_events;
    size_t global_size = data_size / 2;
    size_t local_size = std::min(global_size, work_group_size);
    cl::NDRange global(global_size);
    cl::NDRange local(local_size); 
    cl::EnqueueArgs Args_1(queue_, global, local);
    unsigned stages_number = std::ceil(std::log2(data_size));
    unsigned local_stages = std::log2(local_size);

    cl::Event local_event = bitonic_sort_fast(Args_1, array_buffer, 
                                              cl::Local(2 * local_size * sizeof(int)), 
                                              local_stages, 1);
        
    stage_events.push_back(local_event);

    cl::Event stage_event;

    for (int current_stage = local_stages; current_stage < stages_number; ++current_stage) {
        for (int stage_pass = 0; stage_pass < current_stage + 1; ++stage_pass) {
            bitonic_sort_slow_t bitonic_sort_slow(program_2, "bitonic_sort_global");
            stage_event = bitonic_sort_slow(cl::EnqueueArgs(queue_, global, local), 
                                             array_buffer, current_stage, stage_pass, 1);

            stage_events.push_back(stage_event);
        }
    }

    auto t3 = std::chrono::high_resolution_clock::now();
    cl::copy(queue_, array_buffer, data, data + data_size);
    auto t4 = std::chrono::high_resolution_clock::now();
    data_transfer_time_ += std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count();

    auto wall_time = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t1).count();

    for (auto e : stage_events) {
        pure_gpu_time_ += e.getProfilingInfo<CL_PROFILING_COMMAND_END>() -
                          e.getProfilingInfo<CL_PROFILING_COMMAND_START>();
    }

    return wall_time;
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
