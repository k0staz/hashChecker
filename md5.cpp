#include "md5.h"

inline md5::bit32 md5::F(bit32 x, bit32 y, bit32 z) {
    return x&y | ~x&z;
}

inline md5::bit32 md5::G(bit32 x, bit32 y, bit32 z) {
    return x&z | y&~z;
}

inline md5::bit32 md5::H(bit32 x, bit32 y, bit32 z) {
    return x^y^z;
}

inline md5::bit32 md5::I(bit32 x, bit32 y, bit32 z) {
    return y ^ (x | ~z);
}

inline void md5::FF(bit32 &a, bit32 b, bit32 c, bit32 d, bit32 x, bit32 s, bit32 ac) {
    a = rotate_left(a + F(b, c, d) + x + ac, s) + b;
}

inline void md5::GG(bit32 &a, bit32 b, bit32 c, bit32 d, bit32 x, bit32 s, bit32 ac) {
    a = rotate_left(a + G(b, c, d) + x + ac, s) + b;
}

inline void md5::HH(bit32 &a, bit32 b, bit32 c, bit32 d, bit32 x, bit32 s, bit32 ac) {
    a = rotate_left(a + H(b, c, d) + x + ac, s) + b;
}

inline void md5::II(bit32 &a, bit32 b, bit32 c, bit32 d, bit32 x, bit32 s, bit32 ac) {
    a = rotate_left(a + I(b, c, d) + x + ac, s) + b;
}

inline md5::bit32 md5::rotate_left(bit32 x, int n) {
    return (x << n) | (x >> (32-n));
}

md5::md5() {
    init();
}
md5::md5(std::string text) {
    init();
    calculate(text.begin(), text.end());
}
md5::md5(std::vector<char> file) {
    init();
    calculate(file.begin(), file.end());
}

void md5::init() {
    //load initialization constants
    state[0] = 0x67452301; // А = 01 23 45 67
    state[1] = 0xefcdab89; // В = 89 AB CD EF
    state[2] = 0x98badcfe; // С = FE DC BA 98
    state[3] = 0x10325476; // D = 76 54 32 10

    //Init values used in shifting
    shift_val.insert(shift_val.end(), { 7, 12, 17, 22});
    shift_val.insert(shift_val.end(), { 5,  9, 14, 20});
    shift_val.insert(shift_val.end(), { 4, 11, 16, 23});
    shift_val.insert(shift_val.end(), { 6, 10, 15, 21});

    //Init table of constants
    table_const.insert(table_const.end(), { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee });
    table_const.insert(table_const.end(), { 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 });
    table_const.insert(table_const.end(), { 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be });
    table_const.insert(table_const.end(), { 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 });
    table_const.insert(table_const.end(), { 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa });
    table_const.insert(table_const.end(), { 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 });
    table_const.insert(table_const.end(), { 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed });
    table_const.insert(table_const.end(), { 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a});
    table_const.insert(table_const.end(), { 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c });
    table_const.insert(table_const.end(), { 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 });
    table_const.insert(table_const.end(), { 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 });
    table_const.insert(table_const.end(), { 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 });
    table_const.insert(table_const.end(), { 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 });
    table_const.insert(table_const.end(), { 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 });
    table_const.insert(table_const.end(), { 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 });
    table_const.insert(table_const.end(), { 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 });
}

template<class iterator>
void md5::calculate(iterator strBegin, iterator strEnd) {
    std::vector<bit8> ch_vector;
    std::copy(strBegin, strEnd, std::back_inserter(ch_vector));
    prepare_input(ch_vector);

    for(auto it = ch_vector.begin(), it2 = ch_vector.begin() + 64; it != ch_vector.end(); it+=64, it2 += 64){
        process_block(it, it2);
    }
}

void md5::prepare_input(std::vector<bit8>& ch_vector) {
    std::istringstream iss(std::to_string(ch_vector.size() * 8));
    //Appending 1
    ch_vector.push_back(0x80);
    //Appending zeros
    while (ch_vector.size()%64 != 56) {
        ch_vector.push_back(0x00);
    }
    //Appending original size in 64 bits view
    uint64_t value;
    iss >> value;
    unsigned char ch_value[sizeof(value)];
    std::memcpy(ch_value,&value,sizeof(value));

    for(auto x:ch_value){
        ch_vector.push_back(x);
    }
}

std::vector<md5::bit32> md5::convert_to32(std::vector<bit8>& ch_vector) {
    std::vector<bit32> block;
    for(size_t i = 0; i < ch_vector.size(); i+=4){
        block.push_back(ch_vector[i] | ch_vector[i+1]<<8 | ch_vector[i+2]<<16 | ch_vector[i+3]<<24);
    }
    return block;
}

void md5::process_block(std::vector<bit8>::iterator blBegin, std::vector<bit8>::iterator blEnd) {
    std::vector<bit32> abcd({state[0], state[1], state[2], state[3]});
    std::vector<bit8> temp;
    std::copy(blBegin, blEnd, std::back_inserter(temp));
    std::vector<bit32> converted_bl = convert_to32(temp);
    //Round 1
    for(int i = 0, s = 0, c_v = 0; i != 16; i++, s++, c_v++){
        FF(abcd[0], abcd[1], abcd[2], abcd[3], converted_bl[i], shift_val[s], table_const[c_v]);
        std::rotate(abcd.begin(), abcd.begin()+abcd.size()-1, abcd.end());
        if (s == 3){
            s = -1;
        }
    }

    //Round 2
    int i = 1;
    for (int s = 4, c_v = 16; c_v != 32; c_v++, s++){
        GG(abcd[0], abcd[1], abcd[2], abcd[3], converted_bl[i], shift_val[s], table_const[c_v]);
        std::rotate(abcd.begin(), abcd.begin()+abcd.size()-1, abcd.end());
        if ( i + 5 <= 15) {
            i += 5;
        } else {
            i = i + 5 - 16;
        }
        if (s == 7){
            s = 3;
        }
    }

    //Round 3
    i = 5;
    for (int s = 8, c_v = 32; c_v != 48; c_v++, s++){
        HH(abcd[0], abcd[1], abcd[2], abcd[3], converted_bl[i], shift_val[s], table_const[c_v]);
        std::rotate(abcd.begin(), abcd.begin()+abcd.size()-1, abcd.end());
        if ( i + 3 <= 15) {
            i += 3;
        } else {
            i = i + 3 - 16;
        }
        if (s == 11){
            s = 7;
        }
    }

    //Round 4
    i = 0;
    for (int s = 12, c_v = 48; c_v != 64; c_v++, s++){
        II(abcd[0], abcd[1], abcd[2], abcd[3], converted_bl[i], shift_val[s], table_const[c_v]);
        std::rotate(abcd.begin(), abcd.begin()+abcd.size()-1, abcd.end());
        if ( i + 7 <= 15) {
            i += 7;
        } else {
            i = i + 7 - 16;
        }
        if (s == 15){
            s = 11;
        }
    }

    state[0] += abcd[0];
    state[1] += abcd[1];
    state[2] += abcd[2];
    state[3] += abcd[3];
}

std::string md5::toHEX() const {
    char repr[33];
    for (size_t i = 0, j=0; i<4; i++, j+=4) {
        sprintf(&repr[j*2], "%02X", state[i] & 0xff);
        sprintf(&repr[(j+1)*2], "%02X", (state[i] >> 8) & 0xff);
        sprintf(&repr[(j+2)*2], "%02X", (state[i] >> 16) & 0xff);
        sprintf(&repr[(j+3)*2], "%02X", (state[i] >> 24) & 0xff);
    }
    repr[32] = 0;

    return std::string(repr);

}

std::ostream& operator<<(std::ostream& out, md5 inst)
{
    return out << inst.toHEX();
}
