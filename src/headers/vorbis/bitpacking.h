#pragma once
#ifndef _VORBIS_BITPACKING_H
#define _VORBIS_BITPACKING_H

#include <iostream>
#include <stdint.h>
#include <vector>
#include <string>
#include <bits/stdc++.h>
#include <cstddef>

typedef uint8_t byte;
typedef byte octet;
typedef uint16_t word;
typedef uint32_t dword;
typedef uint64_t qword;
typedef char byteString[8];

namespace Vorbis
{
    constexpr int BITS_IN_BYTE {8};

    class Bitstream
    {
        public:
            Bitstream();
            ~Bitstream();

            //template<typename T>
            //void Insert(T _val);
            void Insert(uint32_t _val);

            //std::vector<char*> mStream;
            std::vector<char*> mStream;

        private:
            //void ModifyBit(byte* val, byte offset, bool set);
            //void ModifyBit(byte* val, byte offset);
            void ModifyBit(char* buf, int offset, bool set);

            int mBitCursor;
    };

    inline std::string DumpBits(size_t val)
    {
        std::string binary = std::bitset<64>(val).to_string();
        return binary.substr(binary.find("1"), binary.size() - 1);
    }
}

#endif // _VORBIS_BITPACKING_H
