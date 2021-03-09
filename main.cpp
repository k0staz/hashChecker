#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "md5.h"
#include "SHA1.h"
#include "SHA256.h"

using namespace std;

vector<char> read_file(const string& path){
    ifstream f;
    f.open(path, ios::binary);
    vector<char> file;
    if (f.is_open()) {
        f.seekg(0, ios::end);
        unsigned int length = f.tellg();
        f.seekg(0, ios::beg);

        char* buffer = new char[length];
        f.read(buffer, length);

        f.close();

        return std::vector<char>(buffer, buffer + length);
    } else {
        return {};
    }
}

void validation(const string& obtained_hash, const string& original_hash, const string& name) {
    if (obtained_hash == original_hash) {
        cout << name + " OK" << endl;
    } else {
        cout << name + " FAIL" << endl;
    }
}


int main(int argc, char *argv[]) {
    ifstream input_file;
    string path;
    if (argc > 1){
        input_file.open(argv[1]);
        path = argv[2];
    } else { //Used for tasting via IDE, in order to use on other pc, change values
        input_file.open("input.txt");
        path = "files_test/";
    }
    while (input_file.good()) {
        string line, file_name, hash_name, hash;
        getline(input_file, line, '\n');
        istringstream iss(line);

        getline(iss, file_name, ' ');
        getline(iss, hash_name, ' ');
        getline(iss, hash, ' ');
        transform(hash.begin(), hash.end(), hash.begin(), [](unsigned char c){ return toupper(c); });

        vector<char> file = read_file(path + file_name);
        if (!file.empty()){
            if (hash_name == "md5") {
                md5 hashed_file(file);
                validation(hashed_file.toHEX(), hash, file_name);
            } else if (hash_name == "sha1") {
                SHA1 hashed_file(file);
                validation(hashed_file.toHEX(), hash, file_name);
            } else if (hash_name == "sha256") {
                SHA256 hashed_file(file);
                validation(hashed_file.toHEX(), hash, file_name);
            } else {
                cout << hash_name + " algorithm is not supported" << endl;
            }
        } else {
            cout << file_name + " NOT FOUND" << endl;
        }

    }


    return 0;
}
