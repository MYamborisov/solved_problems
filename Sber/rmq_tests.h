#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <random>
#include <limits>
#include <cassert>
#include <numeric>

void BruteForceSolution(const std::vector<int>& numbers, std::istream& in, std::ostream& out) {
    std::string cmd;
    int L = 0, R = 0;
    while (in >> cmd) {
        if (cmd == "L") {
            ++L;
        } else if (cmd == "R") {
            ++R;
        }
        out << *std::max_element(numbers.begin() + L, numbers.begin() + R + 1) << std::endl;
    }
}

void Solution(const std::vector<int>& numbers, std::istream& in, std::ostream& out);

void RandomInputTest() {
    for (int i = 0; i < 100; ++i) {
        // generate vector with random numbers
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> count(1, 1000);  // right threshold is reduced from 1e5 to 1e3 to increase speed
        std::uniform_int_distribution<int> distr(INT32_MIN, INT32_MAX);
        std::vector<int> vec;
        std::generate_n(std::back_inserter(vec), count(rng), [distr, &rng]() {
            return distr(rng);
        });
        // generate random input string with restrictions: count_R <= vec.size() - 1 and for any index, the number of preceding L is less or equal to R
        std::uniform_int_distribution<int> distr_R(0, vec.size() - 1);
        int count_R = distr_R(rng);
        std::string input_string;
        int free_R_commands = 0;
        for (int j = 0; j < count_R; ++j) {
            input_string.append("R ");
            // append random number of L in between 0 and (added_R - added_L)
            std::uniform_int_distribution<int> distr_L(0, ++free_R_commands);
            int count_L = distr_L(rng);
            free_R_commands -= count_L;
            while(count_L--) {
                input_string.append("L ");
            }
        }
        // compute result with brute force and optimized functions
        std::istringstream corr_input(input_string), susp_input(input_string);
        std::ostringstream corr_output, susp_output;
        BruteForceSolution(vec, corr_input, corr_output);
        Solution(vec, susp_input, susp_output);
        assert(susp_output.str() == corr_output.str());
    }
}

void EmptyInputTest() {
    std::vector<int> vec = {5};
    std::istringstream input;
    std::ostringstream output;
    Solution(vec, input, output);
    assert(output.str().empty());
}

void FunctionalTest() {
    std::vector<int> vec = {7, 3};
    std::istringstream input("R L");
    std::ostringstream output;
    Solution(vec, input, output);
    assert(output.str() == "7\n3\n");
}

void AscendingTest() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::istringstream input("R R R R L L L L");
    std::ostringstream output;
    Solution(vec, input, output);
    assert(output.str() == "2\n3\n4\n5\n5\n5\n5\n5\n");
}

void DescendingTest() {
    std::vector<int> vec = {0, -1, -2, -3, -4};
    std::istringstream input("R R R R L L L L");
    std::ostringstream output;
    Solution(vec, input, output);
    assert(output.str() == "0\n0\n0\n0\n-1\n-2\n-3\n-4\n");
}

void MaxElementsTest() {
    std::vector<int> vec(100'000);
    std::iota(vec.begin(), vec.end(), 1);
    std::string input_str, output_str;
    for (int i = 2; i < 100'001; ++i) {
        output_str.append(std::to_string(i)+"\n");
        input_str.append("R ");
    }
    std::istringstream input(input_str);
    std::ostringstream output;
    Solution(vec, input, output);
    assert(output.str() == output_str);
}

void Tests() {
    EmptyInputTest();
    FunctionalTest();
    AscendingTest();
    DescendingTest();
    MaxElementsTest();
    RandomInputTest();
    std::cout << "All tests passed!" << std::endl;
}