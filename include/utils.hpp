#ifndef UTILS_HPP
#define UTILS_HPP

#ifndef CL_HPP_TARGET_OPENCL_VERSION 
#define CL_HPP_MINIMUM_OPENCL_VERSION 120 
#define CL_HPP_TARGET_OPENCL_VERSION 120 
#endif 

#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_ENABLE_EXCEPTIONS

#include <CL/opencl.hpp>
#include <vector>
#include <random>
#include <iostream>

#if defined (ANALYSE)
#define dbgs std::cerr
#else 
#define dbgs if (false) std::cerr
#endif

namespace utils {
template <typename Iter> void random_init(Iter begin, Iter end, int min_value, int max_value) {
    static std::mt19937_64 mt_source;
    std::uniform_int_distribution<int> dist(min_value, max_value);
    for (Iter current = begin; current != end; ++current)
        *current = dist(mt_source);
}

bool check_sort(const cl::vector<int>& array);

unsigned int nearest_pow_of_2(unsigned int number);

void print_first_elements(const cl::vector<int>& array, int number_of_elems);
} // namespace utils

#endif // UTILS_HPP
