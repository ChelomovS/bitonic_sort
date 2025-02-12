#include "utils.hpp"

bool check_sort(const cl::vector<int>& array) {
    if (array.empty()) 
        return true; 

    for (size_t i = 1; i < array.size(); ++i) 
        if (array[i] < array[i - 1])
            return false;

    return true; 
}
