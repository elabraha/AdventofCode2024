#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::cout, std::endl, std::string;

int main(int argc, char* argv[])
{
    string fn = argv[1];

    std::ifstream f(fn);

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
    for (int i = filemap_expanded.size() - 1; i >= 0; --i) {
        if (filemap_expanded[i] != ".") {
            // start moving to the first empty space
            cout << "i: " << i << " j: " << j << endl;
            // I have to figure out how to stop once I swapped all locations.
            while (j < filemap_expanded.size()){
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
    for (auto it: filemap_expanded){
        cout << it << " ";
    }
    cout << endl;

    f.close();
    return 0;
}
