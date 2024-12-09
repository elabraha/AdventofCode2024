
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <math.h>
#include <unordered_set>

using std::cout, std::endl, std::string, std::vector, std::pair;

pair<int,int> get_distance(pair<int,int> a, pair<int,int> b) {
    // calculate distance
    int x_diff = a.first - b.first;
    int y_diff = a.second - b.second;
    return pair<int,int> (x_diff, y_diff);
}

bool valid_antinode_spot(int x, int y, char c, size_t rows, size_t cols) {
    if (x < rows && y < cols && x >= 0 && y >= 0) {
        return true;
    } 
    return false;
}

struct SimpleHash {
    size_t operator()(const std::pair<int, int>& p) const {
        return p.first ^ p.second;
    }
};

int gcd(int a, int b) {
    a = abs(a);
    b = abs(b);

    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cerr << "Error: No filename and part provided.\n";
        return 1;
    }
    string fn = argv[1];
    string part = argv[2];
    if (part != "1" or part != "2") {
        std::cerr << "There is no part: " << part << endl;
    } 
    std::ifstream f(fn);

    if (!f.is_open()) {
        std::cerr << "Error: Could not open file: " << fn << endl;
        return 1;
    }

    // keep input as strings no need to complicate anything
    string l;
    vector<string> map;
    std::unordered_map<char, vector<pair<int, int>>> antennas;

    int i = 0;
    while(std::getline(f, l)) {
        map.push_back(l);
        for(int j = 0; j < l.size(); j++) {
            if(l[j] != '.') {
                pair <int,int> coords(i, j);
                if(antennas.contains(l[j])) {
                    antennas[l[j]].push_back(coords);
                } else {
                    antennas[l[j]] = {coords};
                }
            }
        }
        i++;
    }

    for (auto it: antennas) {
        cout << it.first << ": [";
        for (auto v: it.second) {
            cout << "(" << v.first << ", " << v.second << ")";
        }
        cout << "]" <<  endl;
    }
    int rows = map.size();
    int cols = map[0].length();
    std::unordered_set<pair<int,int>, SimpleHash> antinodes;
    // loop through all the frequencies
    for (auto freq: antennas) {
        for (auto antenna1: freq.second) {
            for (auto antenna2: freq.second) {
                if (antenna1 == antenna2) {continue;}
                if (part == "1"){
                    // calculate distances between each antenna to put an antinode
                    pair<int,int> distance = get_distance(antenna2, antenna1);
                    // make a list of all the nodes we need to validate for antinode placement
                    int x = antenna1.first - distance.first;
                    int y = antenna1.second - distance.second;
                    if(valid_antinode_spot(x, y, freq.first, rows, cols)){
                        // add antinode to the map and the sum of antinodes
                        map[x][y] = '#';
                      
                        antinodes.insert(std::make_pair(x,y));
                    }
                }else if (part == "2") {
                    // calculate distances between each antenna to put an antinode
                    pair<int,int> distance = get_distance(antenna2, antenna1);
                    int g = gcd(distance.first, distance.second);
                    cout << "g " << g << endl; 
                    int dx = distance.first/g;
                    int dy = distance.second/g;
                    // go forward and backward
                    int x = antenna1.first - dx;
                    int y = antenna1.second - dy;
                    while(valid_antinode_spot(x, y, freq.first, rows, cols)){
                        if(map[x][y] == '.'){map[x][y] = '#';}
                        antinodes.insert(std::make_pair(x,y));
                        x -= dx;
                        y -= dy;
                    }
                    x = antenna1.first + dx;
                    y = antenna1.second + dy;
                    while(valid_antinode_spot(x, y, freq.first, rows, cols)){
                        if(map[x][y] == '.'){map[x][y] = '#';}
                        antinodes.insert(std::make_pair(x,y));
                        x += dx;
                        y += dy;
                    }
                    antinodes.insert(antenna1);
                }
            }
        }
    }

    // print map
    for (auto line: map) {
        cout << line << endl;
    }

    cout << "---------------------------- END OF MAP ----------------------------" << endl;
    cout << "Number of Antinodes: " << antinodes.size() << endl;

    f.close();

    return 0;
}
