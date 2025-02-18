#ifndef OCL_APP_HPP
#define OCL_APP_HPP

#include "utils.hpp"
#include "config.hpp"

#include <CL/opencl.hpp>

class OclApp {
    private:
    cl::Platform        platform_;
    cl::Context         context_;
    cl::CommandQueue    queue_;
    std::string         kernel_;
    Config              configurations_; 

    static cl::Platform select_platform(); 
    static cl::Context  get_gpu_context(cl_platform_id platform_id);
    static std::string  read_kernel_file(const char* path);
    
    using bitonic_sort_t = cl::KernelFunctor<cl::Buffer, int, int>;

    public:
    OclApp(Config configurations): platform_{select_platform()},
                                   context_{get_gpu_context(platform_())},
                                   queue_{context_, configurations.q_props_},
                                   kernel_{read_kernel_file(configurations.path_)},
                                   configurations_{configurations} {
        cl::string platform_name = platform_.getInfo<CL_PLATFORM_NAME>();
        cl::string profile       = platform_.getInfo<CL_PLATFORM_PROFILE>();
        dbgs << "Selected: " << platform_name << ": " << profile << std::endl;
    }

    int get_local_size() const { return configurations_.local_size_; }
    
    cl::Event bitonic_sort(int* data, size_t data_size_);
};

#endif // OCL_APP_HPP