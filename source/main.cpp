#include "utils.hpp"
#include "config.hpp"
#include "ocl_app.hpp"


#include <iostream>
#include <chrono>
#include <algorithm>
#include <CL/opencl.hpp>
#include <climits> 

int main() try {
    std::chrono::high_resolution_clock::time_point start_time, final_time;
    cl_long wall_time;

    int real_data_size_ = 0;

    std::cin >> real_data_size_;
    if (!std::cin.good() || real_data_size_ < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        std::cout << "Bad input, enter a non-negative integer" << std::endl;
        return EXIT_FAILURE;
    }

    int data_size_for_sort = utils::nearest_pow_of_2(real_data_size_);

    cl::vector<int> array_for_sort;
    array_for_sort.reserve(data_size_for_sort);
    
    Config configurations(data_size_for_sort);
    OclApp application(configurations);

    int element = 0;

    for (auto i = 0; i < real_data_size_; ++i) {
        std::cin >> element;
        if (!std::cin.good()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::cout << "Bad input, enter integer" << std::endl;
            return EXIT_FAILURE;
        }

        array_for_sort.push_back(element);
    }

    // Make size power of 2
    for (auto i = real_data_size_; i < data_size_for_sort; ++i) {
        array_for_sort.push_back(INT_MAX);
    }

    start_time = std::chrono::high_resolution_clock::now();
    
    cl::Event event = application.bitonic_sort(array_for_sort.data(), array_for_sort.size());
    final_time = std::chrono::high_resolution_clock::now();

    wall_time = std::chrono::duration_cast<std::chrono::milliseconds>(final_time - start_time).count();
    dbgs << "GPU wall time measured: " << wall_time << " ms" << std::endl;

    if (utils::check_sort(array_for_sort)) {
        dbgs << "array is sorted" << std::endl;
    } else {
        dbgs << "array is not sorted" << std::endl;
    }

#if defined (COMPARE_CPU) 
    std::vector<int> array_for_sort_CPU(array_for_sort);
    start_time = std::chrono::high_resolution_clock::now();
    std::sort(array_for_sort_CPU.begin(), array_for_sort_CPU.end());
    final_time = std::chrono::high_resolution_clock::now();
    wall_time  = std::chrono::duration_cast<std::chrono::milliseconds>(final_time - start_time).count();
    std::cout << "CPU time measured: " << wall_time << " ms" << std::endl;
#endif 

    dbgs << "All checks passed" << std::endl;

    utils::print_first_elements(array_for_sort, real_data_size_);

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
