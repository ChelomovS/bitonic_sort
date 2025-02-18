#include "utils.hpp"
#include "ocl_app.hpp"
#include "config.hpp"

#include <CL/opencl.hpp>
#include <gtest/gtest.h>
#include <climits>

TEST(SORT, test_1) {
    cl::vector<int> vector_for_test{};
    vector_for_test.push_back(1);
    vector_for_test.push_back(42);
    vector_for_test.push_back(-1);
    vector_for_test.push_back(100);

    int real_data_size_ = vector_for_test.size();
    int data_size_for_sort = utils::nearest_pow_of_2(real_data_size_);

    for (auto i = real_data_size_; i < data_size_for_sort; ++i) {
        vector_for_test.push_back(INT_MAX);
    }

    Config configurations(data_size_for_sort, "../../kernel_for_sort.cl");
    OclApp application(configurations);

    cl::Event event = application.bitonic_sort(vector_for_test.data(), vector_for_test.size());
    ASSERT_EQ(true, utils::check_sort(vector_for_test));
} 


TEST(SORT, test_2) {
    cl::vector<int> vector_for_test{};
    vector_for_test.push_back(1);
    vector_for_test.push_back(42);
    vector_for_test.push_back(-1);
    vector_for_test.push_back(100);
    vector_for_test.push_back(300); 
    vector_for_test.push_back(-300); 
    vector_for_test.push_back(-300); 

    int real_data_size_ = vector_for_test.size();
    int data_size_for_sort = utils::nearest_pow_of_2(real_data_size_);

    for (auto i = real_data_size_; i < data_size_for_sort; ++i) {
        vector_for_test.push_back(INT_MAX);
    }

    Config configurations(data_size_for_sort, "../../kernel_for_sort.cl");
    OclApp application(configurations);

    cl::Event event = application.bitonic_sort(vector_for_test.data(), vector_for_test.size());
    ASSERT_EQ(true, utils::check_sort(vector_for_test));
} 

TEST(SORT, test_3) {
    cl::vector<int> vector_for_test{};
    vector_for_test.push_back(-1);
    vector_for_test.push_back(-42);
    vector_for_test.push_back(-1);
    vector_for_test.push_back(-100);
    vector_for_test.push_back(-300); 
    vector_for_test.push_back(-300); 
    vector_for_test.push_back(-300); 
    vector_for_test.push_back(3000000); 
    vector_for_test.push_back(0); 

    int real_data_size_ = vector_for_test.size();
    int data_size_for_sort = utils::nearest_pow_of_2(real_data_size_);

    for (auto i = real_data_size_; i < data_size_for_sort; ++i) {
        vector_for_test.push_back(INT_MAX);
    }

    Config configurations(data_size_for_sort, "../../kernel_for_sort.cl");
    OclApp application(configurations);

    cl::Event event = application.bitonic_sort(vector_for_test.data(), vector_for_test.size());
    ASSERT_EQ(true, utils::check_sort(vector_for_test));
} 

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}