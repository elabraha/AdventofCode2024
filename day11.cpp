
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>

using std::cout, std::endl, std::string, std::vector, std::unordered_map, std::pair;

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        return h1 ^ h2;  
    }
};
// Try to go back and understand this one as best as you can!
long long count_stones(long long blinks, long long stone, unordered_map<pair<long long,long long>,long long, pair_hash> &split_count) {
    // cout << "blinks: " << blinks << " stone: " << stone << endl;;
    if (blinks > 0) {
        pair<long long,long long> stone_at_blinks = {stone, blinks};
        if (split_count.find(stone_at_blinks) == split_count.end()) {
            string str_stone = std::to_string(stone);
            if (stone == 0){
                // convert to 1 so add to memo
                split_count[stone_at_blinks] = count_stones(blinks - 1, 1, split_count);
            } else if (str_stone.length() % 2 == 0){
                // start recursive split
                size_t half = str_stone.length() / 2;
                long long left_half = std::stoll(str_stone.substr(0, half));
                long long right_half = std::stoll(str_stone.substr(half, str_stone.length()));
                split_count[stone_at_blinks] = count_stones(blinks-1, left_half, split_count) + count_stones(blinks-1, right_half, split_count);
            } else {
                // multiply by 2024 and call the count again
                split_count[stone_at_blinks] = count_stones(blinks-1, stone * 2024, split_count);
            }
        }
        // cout << "count " << split_count[stone_at_blinks] << endl;
        return split_count[stone_at_blinks];
    } 
    // cout << "base case " << endl;
    return 1;
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cerr << "Error: No filename and number of blinks.\n";
        return 1;
    }
    string fn = argv[1];
    long long blinks = std::atoll(argv[2]);

    std::ifstream f(fn);

    if (!f.is_open()) {
        std::cerr << "Error: Could not open file: " << fn << endl;
        return 1;
    }

    string str_stones;
    std::getline(f, str_stones);
    vector<long long> stones;
    long long og_blinks = blinks;

    std::stringstream iss(str_stones);
    long long stone;
    unordered_map<long long, long long> stone_map;
    while (iss >> stone) {
        stones.push_back(stone);
        if (stone_map.contains(stone)) {
            stone_map[stone]++;
        } else {
            stone_map[stone] = 1;
        }
    }

    // ######################### PART 1: ###########################
    // vector<long> result_stones;
    // while(blinks > 0) {
    //     for (auto stone: stones){
    //         string str_stone = std::to_string(stone);
    //         if (stone == 0){
    //             result_stones.push_back(1);
    //         } else if (str_stone.length() % 2 == 0){
    //             size_t half = str_stone.length() / 2;
    //             long left_half = std::stoi(str_stone.substr(0, half));
    //             result_stones.push_back(left_half);
    //             long right_half = std::stol(str_stone.substr(half, str_stone.length()));
    //             result_stones.push_back(right_half);
    //         } else {
    //             result_stones.push_back(stone * 2024);
    //         }
    //     }
    //     stones = {};
    //     std::copy(result_stones.begin(), result_stones.end(), std::back_inserter(stones));
    //     result_stones = {};
    //     blinks--;
    // }
    // cout << "Number of stones after " << og_blinks << " blinks: " << stones.size() << endl;
    // cout << "Order of stones after " << og_blinks << " blinks: ";
    // for (size_t i = 0; i < stones.size(); i++) {
    //     cout << stones[i] << " ";
    // }
    // cout << endl;

    // ########################### PART 2: #############################
    // long long count = 0;

    // for (auto stone: stones){
    //     unordered_map<pair<long long,long long>,long long, pair_hash> split_count = {};
    //     // cout << "stone " << stone << endl; 
    //     count += count_stones(blinks, stone, split_count);
    // }

    // HERE I SHOULD TRY MEMOIZING ITERATIVELY!!
    // Try to figure out why this and the recursive one works! because I just tried things until they worked
    // and sorta made sense so really get an understanding of this one.
    unordered_map<long long, long long> result_stones;
    while(blinks > 0) {
        result_stones = {};
        for (const auto& [stone, count]: stone_map){
            string str_stone = std::to_string(stone);
            if (stone == 0){
                result_stones[1] += stone_map[stone]; // why can't this just be plus 1?
            } else if (str_stone.length() % 2 == 0){
                size_t half = str_stone.length() / 2;
                long left_half = std::stoi(str_stone.substr(0, half));
                result_stones[left_half] += stone_map[stone];
                long right_half = std::stol(str_stone.substr(half, str_stone.length()));
                result_stones[right_half] += stone_map[stone];
            } else {
                result_stones[stone * 2024] += stone_map[stone];
            }
        }
        stone_map = result_stones;
        blinks--;
    }

    long long count = 0;
    for (const auto &stone: stone_map)
    {
        count += stone.second;
    }
    
    cout << "Number of stones after " << og_blinks << " blinks: " << count << endl;
    f.close();
    return 0;
}
