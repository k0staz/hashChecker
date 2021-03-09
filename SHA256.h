#ifndef HASHCHECKER_SHA256_H
#define HASHCHECKER_SHA256_H

#include "SHA.h"

/**
 * Implementation of SHA256 hash-algorithm.
 *
 * Inherits two functions from SHA class (prepare_input and convert_to32).
 *
 */
class SHA256 : SHA {
public:
    SHA256();
    SHA256(std::string text);
    SHA256(std::vector<char> file);
    std::string toHEX() const;
    friend std::ostream& operator<<(std::ostream&, SHA256 inst);

private:
    void init(); //Set up variables for processing algorithms

    //First 32 bits of the fractional parts of the square roots of the first 8 primes 2..19
    bit32 state[8];

    //first 32 bits of the fractional parts of the cube roots of the first 64 primes 2..311
    std::vector<bit32> constants;

    template<class iterator>
    void calculate(iterator strBegin, iterator strEnd);

    //Processing 512-bits blocks
    void process_block(std::vector<bit8>::iterator blBegin, std::vector<bit8>::iterator blEnd);

    //Extend the first 16 words into the remaining 48 words w[16..63] of the message schedule array:
    static inline std::vector<bit32> convert_to_w(std::vector<bit32>& ch_vector);

    //Shift to the left for n bits for given 32-bits argument
    static inline bit32 rotate_right(bit32 x, int n);
};


#endif //HASHCHECKER_SHA256_H
