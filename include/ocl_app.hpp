#ifndef OCL_APP_HPP
#define OCL_APP_HPP

#include "utils.hpp"
#include "config.hpp"

#include <CL/opencl.hpp>

class OclApp {
    public:
    Config              configurations_; 
    cl::Platform        platform_;
    cl::Context         context_;
    cl::CommandQueue    queue_;
    std::string         kernel_;
    cl_ulong            pure_gpu_time_      = 0; 
    cl_ulong            data_transfer_time_ = 0;
    size_t              work_group_size     = 1;
    cl::Device          device_;
    std::string         sort_fast;
    std::string         sort_default;

    public:   
    static cl::Platform select_platform();
    static cl::Context  get_gpu_context(cl_platform_id platform_id);
    static std::string  read_kernel_file(const char* path);

    public:
    OclApp(Config configurations): 
        configurations_{configurations},
        platform_{select_platform()},
        context_{get_gpu_context(platform_())},
        queue_{context_, configurations.q_props_},
        device_{context_.getInfo<CL_CONTEXT_DEVICES>()[0]},
        sort_fast{read_kernel_file(configurations.path_to_fast_)},
        sort_default{read_kernel_file(configurations.path_to_slow_)} { 
            work_group_size = device_.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
            cl::string platform_name = platform_.getInfo<CL_PLATFORM_NAME>();
            cl::string profile       = platform_.getInfo<CL_PLATFORM_PROFILE>();
            work_group_size          = device_.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
            dbgs << "Selected: " << platform_name << ": " << profile << std::endl;
        }

    // functors 
    using bitonic_sort_fast_t = cl::KernelFunctor<cl::Buffer, cl::LocalSpaceArg, int, int>;
    using bitonic_sort_slow_t = cl::KernelFunctor<cl::Buffer, int, int, int>;

    cl_ulong get_pure_gpu_time() const { return pure_gpu_time_; }
    cl_ulong get_data_transfer_time() const { return data_transfer_time_; } 
    
    long int bitonic_sort(int* data, size_t data_size);
};

#endif // OCL_APP_HPP