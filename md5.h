#ifndef HASHCHECKER_MD5_H
#define HASHCHECKER_MD5_H


#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>


class md5 {
public:
    md5();
    md5(std::string text);
    md5(std::vector<char> file);
    std::string toHEX() const;
    friend std::ostream& operator<<(std::ostream&, md5 inst);
private:
    void init(); //Set up variables for processing algorithms
    //Assuming that char is 8 bits and int is 32 bits, rename them for convenience
    typedef unsigned char bit8;
    typedef unsigned int bit32;

    bit32 state[4]; //Buffer for A,B,C,D
    std::vector<bit32> shift_val;
    std::vector<bit32> table_const;

    //Processing 512-bits blocks
    void process_block(std::vector<bit8>::iterator blBegin, std::vector<bit8>::iterator blEnd);

    //Calculates MD5
    template<class iterator>
    void calculate(iterator strBegin, iterator strEnd);

    //Does following steps: 1.Append 1; 2. Append 0 till L mod 512 == 448 is true; 3.Append original length
    void prepare_input(std::vector<bit8>& ch_vector);

    //Basic MD5 functions
    static inline bit32 F(bit32 x, bit32 y, bit32 z);
    static inline bit32 G(bit32 x, bit32 y, bit32 z);
    static inline bit32 H(bit32 x, bit32 y, bit32 z);
    static inline bit32 I(bit32 x, bit32 y, bit32 z);
    static inline void FF(bit32& a, bit32 b, bit32 c, bit32 d, bit32 x, bit32 s, bit32 ac);
    static inline void GG(bit32& a, bit32 b, bit32 c, bit32 d, bit32 x, bit32 s, bit32 ac);
    static inline void HH(bit32& a, bit32 b, bit32 c, bit32 d, bit32 x, bit32 s, bit32 ac);
    static inline void II(bit32& a, bit32 b, bit32 c, bit32 d, bit32 x, bit32 s, bit32 ac);


    //Rotate to the left for n bits for given 32-bits argument
    static inline bit32 rotate_left(bit32 x, int n);
    //Convert 8bits array into 32 bits array (breaks a block of 512 bits into 16 blocks per 32 bits)
    static inline std::vector<bit32> convert_to32(std::vector<bit8>& ch_vector);
    //Converting to hexadecimal format
};


#endif //HASHCHECKER_MD5_H
