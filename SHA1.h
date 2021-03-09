#ifndef HASHCHECKER_SHA1_H
#define HASHCHECKER_SHA1_H

#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

class SHA1 {
public:
    SHA1();
    SHA1(std::string text);
    SHA1(std::vector<char> file);
    std::string toHEX() const;
    friend std::ostream& operator<<(std::ostream&, SHA1 inst);

private:
    void init(); //Set up variables for processing algorithms
    //Assuming that char is 8 bits and int is 32 bits, rename them for convenience
    typedef unsigned char bit8;
    typedef unsigned int bit32;

    bit32 state[5];
    bit32 constantsK[4];

    //Calculates SHA1
    template<class iterator>
    void calculate(iterator strBegin, iterator strEnd);

    //Processing 512-bits blocks
    void process_block(std::vector<bit8>::iterator blBegin, std::vector<bit8>::iterator blEnd);

    //Does following steps: 1.Append 1; 2. Append 0 till L mod 512 == 448 is true; 3.Append original length
    static inline void prepare_input(std::vector<bit8>& ch_vector);

    //Convert 8bits array into 32 bits array (breaks a block of 512 bits into 16 blocks per 32 bits)
    static inline std::vector<bit32> convert_to32(std::vector<bit8>& ch_vector);

    //Does transformation from M to W (from 16 pieces of 32-bits to 80 pieces of 32-bits)
    static inline std::vector<bit32> convert_to_w(std::vector<bit32>& ch_vector);

    //4 non-linear basic operations Ft
    static inline bit32 F1(bit32 m, bit32 l, bit32 k);
    static inline bit32 F2(bit32 m, bit32 l, bit32 k);
    static inline bit32 F3(bit32 m, bit32 l, bit32 k);
    static inline bit32 F4(bit32 m, bit32 l, bit32 k);

    //Main operation during calculation
    static inline void FF(bit32 &a, bit32 &b, bit32 &c, bit32 &d, bit32 &e, bit32 w, bit32 k, int n);

    //Rotate to the left for n bits for given 32-bits argument
    static inline bit32 rotate_left(bit32 x, int n);
};


#endif //HASHCHECKER_SHA1_H
