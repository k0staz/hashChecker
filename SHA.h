#ifndef HASHCHECKER_SHA_H
#define HASHCHECKER_SHA_H

#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

class SHA {
private:

protected:
    //Assuming that char is 8 bits and int is 32 bits, rename them for convenience
    typedef unsigned char bit8;
    typedef unsigned int bit32;

    //Pre-processing (Padding)
    virtual void prepare_input(std::vector<bit8>& ch_vector);

    //Convert 8bits array into 32 bits array (breaks a block of 512 bits into 16 blocks per 32 bits (big-endian))
    virtual std::vector<bit32> convert_to32(std::vector<bit8>& ch_vector);
};


#endif //HASHCHECKER_SHA_H
