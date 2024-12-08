#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <math.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: No filename provided.\n";
        return 1;
    }
    std::string filename = argv[1];
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return 1;
    }

    std::string line;

    std::vector<int> left_list, right_list;

    std::unordered_map<int,int> appearances;

    while (std::getline(inputFile, line)) {
        int left, right;
        std::stringstream ss(line);
        ss >> left >> right;
        left_list.push_back(left);
        right_list.push_back(right);
        // yayy C++20
        if(appearances.contains(right)) {
            appearances[right] += 1;
        } else {
            appearances[right] = 1;
        }
    }
    //part 2 don't sort
    long similarity_score = 0;
    for (auto left: left_list) {
        if(appearances.contains(left)) {
            // std::cout << left << " " << appearances[left] << " " << left*appearances[left] << std::endl;
            similarity_score += (left*appearances[left]);
        }
    }

    // part one sorting and calculated distance between numbers
    std::sort(left_list.begin(), left_list.end());
    std::sort(right_list.begin(), right_list.end());
    long sum = 0;
    for (int i = 0; i < left_list.size(); i++) {
        // std::cout << left_list[i] << " " << right_list[i] << std::endl;
        sum += std::abs(left_list[i] - right_list[i]);
    }
    std::cout << "part 1: " << sum << std::endl;
    std::cout << "part 2: " << similarity_score << std::endl;

    inputFile.close();
    return 0;
}
