#include "SHA256.h"


SHA256::SHA256() {
    init();
}
SHA256::SHA256(std::string text) {
    init();
    calculate(text.begin(), text.end());
}
SHA256::SHA256(std::vector<char> file) {
    init();
    calculate(file.begin(), file.end());
}

void SHA256::init() {
    //Initialize hash values
    state[0] = 0x6a09e667;
    state[1] = 0xbb67ae85;
    state[2] = 0x3c6ef372;
    state[3] = 0xa54ff53a;
    state[4] = 0x510e527f;
    state[5] = 0x9b05688c;
    state[6] = 0x1f83d9ab;
    state[7] = 0x5be0cd19;

    //Initialize vector of round constants:
    constants.insert(constants.end(), {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
                                       0x923f82a4, 0xab1c5ed5});
    constants.insert(constants.end(), {0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe,
                                       0x9bdc06a7, 0xc19bf174});
    constants.insert(constants.end(), {0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa,
                                       0x5cb0a9dc, 0x76f988da});
    constants.insert(constants.end(), {0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
                                       0x06ca6351, 0x14292967});
    constants.insert(constants.end(), {0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb,
                                       0x81c2c92e, 0x92722c85});
    constants.insert(constants.end(), {0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624,
                                       0xf40e3585, 0x106aa070});
    constants.insert(constants.end(), {0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
                                       0x5b9cca4f, 0x682e6ff3});
    constants.insert(constants.end(), {0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb,
                                       0xbef9a3f7, 0xc67178f2});
}

template<class iterator>
void SHA256::calculate(iterator strBegin, iterator strEnd) {
    std::vector<bit8> ch_vector;
    std::copy(strBegin, strEnd, std::back_inserter(ch_vector));
    prepare_input(ch_vector);

    for(auto it = ch_vector.begin(), it2 = ch_vector.begin() + 64; it != ch_vector.end(); it+=64, it2 += 64){
        process_block(it, it2);
    }
}

void SHA256::process_block(std::vector<bit8>::iterator blBegin, std::vector<bit8>::iterator blEnd) {
    //    a := h0 b := h1 c := h2 d := h3 e := h4 f := h5 g := h6 h := h7
    std::vector<bit32> a_h({state[0],state[1],state[2],state[3],state[4],state[5],state[6],state[7]});

    std::vector<bit8> temp;
    std::copy(blBegin, blEnd, std::back_inserter(temp));

    std::vector<bit32> converted = convert_to32(temp);
    std::vector<bit32> w = convert_to_w(converted);

    for (int i = 0; i <= 63; i++) {
        bit32 sigma0 = rotate_right(a_h[0], 2) ^ rotate_right(a_h[0], 13) ^ rotate_right(a_h[0], 22);
        bit32 Ma = a_h[0]&a_h[1] ^ a_h[0]&a_h[2] ^ a_h[1]&a_h[2];
        bit32 t2 = sigma0 + Ma;
        bit32 sigma1 = rotate_right(a_h[4], 6) ^ rotate_right(a_h[4], 11) ^ rotate_right(a_h[4], 25);
        bit32 Ch = a_h[4]&a_h[5] ^ (~a_h[4]&a_h[6]);
        bit32 t1 = a_h[7] + sigma1 + Ch + constants[i] + w[i];

        a_h[7] = a_h[6];      //h := g
        a_h[6] = a_h[5];      //g := f
        a_h[5] = a_h[4];      //f := e
        a_h[4] = a_h[3] + t1; //e := d + t1
        a_h[3] = a_h[2];      //d := c
        a_h[2] = a_h[1];      //c := b
        a_h[1] = a_h[0];      //b := a
        a_h[0] = t1 + t2;     //a := t1 + t2
    }

    //Add the compressed chunk to the current hash value:
    state[0] += a_h[0];
    state[1] += a_h[1];
    state[2] += a_h[2];
    state[3] += a_h[3];
    state[4] += a_h[4];
    state[5] += a_h[5];
    state[6] += a_h[6];
    state[7] += a_h[7];
}

std::vector<SHA256::bit32> SHA256::convert_to_w(std::vector<bit32> &ch_vector) {
    std::vector<bit32> w;
    std::copy(ch_vector.begin(), ch_vector.end(), std::back_inserter(w)); //Wt = Mt 0 <=t <= 15

    //Extend the first 16 words into the remaining 48 words w[16..63] of the message schedule array:
    for (int i = 16; i <= 63; i++){
        bit32 s0 = rotate_right(w[i-15], 7) ^ rotate_right(w[i-15], 18) ^ (w[i-15] >> 3);
        bit32 s1 = rotate_right(w[i-2], 17) ^ rotate_right(w[i-2], 19) ^ (w[i-2] >> 10);
        w.push_back(w[i-16] + s0 + w[i-7] + s1);
    }

    return w;
}


SHA256::bit32 SHA256::rotate_right(bit32 x, int n) {
    return (x >> n) | (x << (32-n));
}

std::string SHA256::toHEX() const {
    char repr[65];
    for (size_t i = 0, j=0; i<8; i++, j+=4) {
        sprintf(&repr[j*2], "%02X", __builtin_bswap32(state[i]) & 0xff);
        sprintf(&repr[(j+1)*2], "%02X", (__builtin_bswap32(state[i]) >> 8) & 0xff);
        sprintf(&repr[(j+2)*2], "%02X", (__builtin_bswap32(state[i]) >> 16) & 0xff);
        sprintf(&repr[(j+3)*2], "%02X", (__builtin_bswap32(state[i]) >> 24) & 0xff);
    }
    repr[64] = 0;

    return std::string(repr);
}

std::ostream& operator<<(std::ostream& out, SHA256 inst)
{
    return out << inst.toHEX();
}