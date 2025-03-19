#include "utils.hpp"

#include <iostream>

bool utils::check_sort(const cl::vector<int>& array) {
    if (array.empty()) 
        return true; 

    for (size_t i = 1; i < array.size(); ++i) 
        if (array[i] < array[i - 1])
            return false;

    return true; 
}

unsigned int utils::nearest_pow_of_2(unsigned int number) {
    if (number <= 1) {
        return 1;
    }

    number -= 1;

    number |= number >> 1;
    number |= number >> 2;
    number |= number >> 4;
    number |= number >> 8;
    number |= number >> 16;

    return number + 1;
}

void utils::print_first_elements(const cl::vector<int>& array, int number_of_elems) {
    std::cout << "Вывод массива: ";
    for (auto i = 0; i < number_of_elems; ++i) {
        std::cout << array[i] << " ";
    }
}