#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "md5.h"


int main() {
    std::string name = "dsa.txt";
    std::ifstream f;
    f.open(name, std::ios::binary);
    std::vector<char> file;
    if (f.is_open()) {
        f.seekg(0, std::ios::end);
        unsigned int length = f.tellg();
        f.seekg(0, std::ios::beg);

        char* buffer = new char[length];
        f.read(buffer, length);

        f.close();

        file = std::vector<char>(buffer, buffer + length);
    }
    md5 checker(file);
    std::cout << checker << std::endl;
    return 0;
}
