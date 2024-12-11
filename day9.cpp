#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <numeric>

using std::cout, std::endl, std::string, std::vector;

struct Block {
    int start;
    int size;
    string element;
};


int main(int argc, char* argv[])
{
    string fn = argv[1];

    std::ifstream f(fn);

    long checksum = 0;
    bool part1 = false;


    if (part1) {
        std::vector<string> filemap_expanded;

        char ch;
        bool free_space = false;
        int file_id = 0;
        while (f.peek()  != EOF) { // Read one character at a time
            char ch = f.get();
            int num = ch - '0';
            cout << ch << " " << num << endl;
            if (free_space) {
                for (int ix = 0; ix < num; ix++){filemap_expanded.push_back(".");}
            } else {
                for (int ix = 0; ix < num; ix++){
                    cout << "file id: " << file_id << endl;
                    filemap_expanded.push_back(std::to_string(file_id));
                }
                file_id++;
            }
            free_space = !free_space;
        }

        cout << "expanded file map: ";
        for (auto it: filemap_expanded){
            cout << it << " ";
        }
        cout << endl;
        int j = 0;
        for (int i = filemap_expanded.size() - 1; i >= j; --i) {
            if (filemap_expanded[i] != ".") {
                // start moving to the first empty space
                cout << "i: " << i << " j: " << j << endl;
                // I have to figure out how to stop once I swapped all locations.
                while (j < i){
                    if (filemap_expanded[j] == ".") {
                        // swap file chunk at i with chunk at j
                        string temp = filemap_expanded[i];
                        filemap_expanded[i] = filemap_expanded[j];
                        filemap_expanded[j] = temp;
                        break;
                    }
                    j++;
                }
            }
            
        }
        cout << "expanded file map after compacting disk: ";
        for (int i = 0; i < filemap_expanded.size(); i++){
            cout << filemap_expanded[i] << " ";
            if (filemap_expanded[i] != ".") {
                checksum += (std::stoi(filemap_expanded[i]) * i);
            }
        }
        cout << endl;
    } else { 

        char ch;
        int file_id = 0;

        vector<Block>freespace;
        vector<Block>files;
        int idx;
        int i = 0;
        while (true) {
            char ch = f.get();
            if (f.peek() == EOF) {break;}
            int num = ch - '0';
            Block bl;
            bl.size = num;
            bl.start = idx;
            if (i % 2 == 1) {
                // free space
                bl.element = ".";
                freespace.push_back(bl);
            } else {
                //not free space
                bl.element = std::to_string(file_id);
                files.push_back(bl);
                // cout << file_id << " " << bl.size << " " << bl.start << endl;
                file_id++;
            }
            idx+=num;
            i++;
        }
        // re-organize full files into first available free space
        for (int i = files.size() - 1; i >= 0; --i) {
            for (int j = 0; j <= freespace.size(); j++) {
                if (freespace[j].size >= files[i].size && freespace[j].start < files[i].start) {
                    // cout << " you should not be here " << files[i].element << " " << freespace[j].start << endl;
                    files[i].start = freespace[j].start;
                    freespace[j].size = freespace[j].size - files[i].size;
                    freespace[j].start = freespace[j].start + files[i].size;
                    break;
                }
            }
        }

        // custom sort for files based on starting index

        auto BlockSort = [] (Block const& b1, Block const& b2) -> bool {
            return b1.start < b2.start;
        };

        std::sort(files.begin(), files.end(), BlockSort);

        for(auto block: files) {
            //  cout << "files for each block: " << block.element << " " << block.start << " " << block.size << endl; 
            for (int i = block.start; i < block.start + block.size; i++) {
                
                checksum += i * std::stoi(block.element);
            }
        }
    }

    cout << "file map checksum: " << checksum << endl;

    f.close();
    return 0;
}
