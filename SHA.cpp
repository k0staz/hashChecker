#include "SHA.h"

void SHA::prepare_input(std::vector<bit8> &ch_vector) {
    std::istringstream iss(std::to_string(ch_vector.size() * 8));
    //Appending 1
    ch_vector.push_back(0x80);
    //Appending zeros
    while (ch_vector.size()%64 != 56) {
        ch_vector.push_back(0x00);
    }
    //Appending original size in 64 bits view in big-endian format
    uint64_t value;
    iss >> value;
    value = __builtin_bswap64(value);
    unsigned char ch_value[sizeof(value)];
    std::memcpy(ch_value,&value,sizeof(value));

    for(auto x:ch_value){
        ch_vector.push_back(x);
    }
}

std::vector<SHA::bit32> SHA::convert_to32(std::vector<bit8> &ch_vector) {
    std::vector<bit32> block;
    for(size_t i = 0; i < ch_vector.size(); i+=4){
        block.push_back(__builtin_bswap32(ch_vector[i] | ch_vector[i+1]<<8 | ch_vector[i+2]<<16 | ch_vector[i+3]<<24));
    }
    return block;
}