#include "SHA1.h"

SHA1::SHA1() {
    init();
}
SHA1::SHA1(std::string text) {
    init();
    calculate(text.begin(), text.end());
}
SHA1::SHA1(std::vector<char> file) {
    init();
    calculate(file.begin(), file.end());
}

void SHA1::init() {
    //load initialization constants
    state[0] = 0x67452301; //A
    state[1] = 0xEFCDAB89; //B
    state[2] = 0x98BADCFE; //C
    state[3] = 0x10325476; //D
    state[4] = 0xC3D2E1F0; //E

    //load K constants
    constantsK[0] = 0x5A827999;
    constantsK[1] = 0x6ED9EBA1;
    constantsK[2] = 0x8F1BBCDC;
    constantsK[3] = 0xCA62C1D6;

}

template<class iterator>
void SHA1::calculate(iterator strBegin, iterator strEnd) {
    std::vector<bit8> ch_vector;
    std::copy(strBegin, strEnd, std::back_inserter(ch_vector));
    prepare_input(ch_vector);

    for(auto it = ch_vector.begin(), it2 = ch_vector.begin() + 64; it != ch_vector.end(); it+=64, it2 += 64){
        process_block(it, it2);
    }
}

void SHA1::process_block(std::vector<bit8>::iterator blBegin, std::vector<bit8>::iterator blEnd) {
    std::vector<bit32> abcde({state[0], state[1], state[2], state[3], state[4]});

    std::vector<bit8> temp;
    std::copy(blBegin, blEnd, std::back_inserter(temp));
    std::vector<bit32> converted_to_m = convert_to32(temp);
    std::vector<bit32> converted_to_w = convert_to_w(converted_to_m);

    for (int i = 0; i <=79; i++) {
        if (i <= 19 && i >=0) {
            FF1(abcde[0], abcde[1], abcde[2], abcde[3], abcde[4], converted_to_w[i], constantsK[0]);
        } else if (i <= 39 && i >=20) {
            FF2(abcde[0], abcde[1], abcde[2], abcde[3], abcde[4], converted_to_w[i], constantsK[1]);
        } else if (i <= 59 && i >=40) {
            FF3(abcde[0], abcde[1], abcde[2], abcde[3], abcde[4], converted_to_w[i], constantsK[2]);
        } else if (i <= 79 && i >= 60) {
            FF4(abcde[0], abcde[1], abcde[2], abcde[3], abcde[4], converted_to_w[i], constantsK[3]);
        }
    }

    state[0] += abcde[0];
    state[1] += abcde[1];
    state[2] += abcde[2];
    state[3] += abcde[3];
    state[4] += abcde[4];
}

std::vector<SHA1::bit32> SHA1::convert_to32(std::vector<bit8> &ch_vector) {
    std::vector<bit32> block;
    for(size_t i = 0; i < ch_vector.size(); i+=4){
        block.push_back(__builtin_bswap32(ch_vector[i] | ch_vector[i+1]<<8 | ch_vector[i+2]<<16 | ch_vector[i+3]<<24));
    }
    return block;
}

std::vector<SHA1::bit32> SHA1::convert_to_w(std::vector<bit32> &ch_vector) {
    std::vector<bit32> w;
    std::copy(ch_vector.begin(), ch_vector.end(), std::back_inserter(w)); //Wt = Mt 0 <=t <= 15
    for (int i = 16; i <= 79; i++) {
        w.push_back(shift_left(w[i-3]^w[i-8]^w[i-14]^w[i-16], 1));
    }
    return w;
}

void SHA1::prepare_input(std::vector<bit8> &ch_vector) {
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

SHA1::bit32 SHA1::F1(bit32 m, bit32 l, bit32 k) {
    return m&l | ~m&k;
}

SHA1::bit32 SHA1::F2(bit32 m, bit32 l, bit32 k) {
    return m^l^k;
}

SHA1::bit32 SHA1::F3(bit32 m, bit32 l, bit32 k) {
    return m&l | m&k | l&k;
}

SHA1::bit32 SHA1::F4(bit32 m, bit32 l, bit32 k) {
    return m^l^k;
}

//TODO: Put all functions in 1
void SHA1::FF1(bit32 &a, bit32 &b, bit32 &c, bit32 &d, bit32 &e, bit32 w, bit32 k) {
    bit32 temp = shift_left(a, 5) + F1(b, c, d) + e + w + k;
    e = d;
    d = c;
    c = shift_left(b, 30);
    b = a;
    a = temp;
}

void SHA1::FF2(bit32 &a, bit32 &b, bit32 &c, bit32 &d, bit32 &e, bit32 w, bit32 k) {
    bit32 temp = shift_left(a, 5) + F2(b, c, d) + e + w + k;
    e = d;
    d = c;
    c = shift_left(b, 30);
    b = a;
    a = temp;
}

void SHA1::FF3(bit32 &a, bit32 &b, bit32 &c, bit32 &d, bit32 &e, bit32 w, bit32 k) {
    bit32 temp = shift_left(a, 5) + F3(b, c, d) + e + w + k;
    e = d;
    d = c;
    c = shift_left(b, 30);
    b = a;
    a = temp;
}

void SHA1::FF4(bit32 &a, bit32 &b, bit32 &c, bit32 &d, bit32 &e, bit32 w, bit32 k) {
    bit32 temp = shift_left(a, 5) + F4(b, c, d) + e + w + k;
    e = d;
    d = c;
    c = shift_left(b, 30);
    b = a;
    a = temp;
}

SHA1::bit32 SHA1::shift_left(bit32 x, int n) {
    return (x << n) | (x >> (32-n));
}

std::string SHA1::toHEX() const {
    bit32 bg[] = {__builtin_bswap32(state[0]),__builtin_bswap32(state[1]),__builtin_bswap32(state[2]),
                  __builtin_bswap32(state[3]),__builtin_bswap32(state[4]),};

    char repr[41];
    for (size_t i = 0, j=0; i<5; i++, j+=4) {
        sprintf(&repr[j*2], "%02X", bg[i] & 0xff);
        sprintf(&repr[(j+1)*2], "%02X", (bg[i] >> 8) & 0xff);
        sprintf(&repr[(j+2)*2], "%02X", (bg[i] >> 16) & 0xff);
        sprintf(&repr[(j+3)*2], "%02X", (bg[i] >> 24) & 0xff);;
    }
    repr[40] = 0;

    return std::string(repr);
}

std::ostream& operator<<(std::ostream& out, SHA1 inst)
{
    return out << inst.toHEX();
}