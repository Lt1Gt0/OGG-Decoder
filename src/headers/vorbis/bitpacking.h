#pragma once
#ifndef _VORBIS_BITPACKING_H
#define _VORBIS_BITPACKING_H

#include <stdint.h>
#include <vector>

typedef uint8_t byte;
typedef byte octet;
typedef uint16_t word;
typedef uint32_t dword;
typedef uint64_t qword;

namespace Vorbis
{
    class Bitstream
    {
        public:
            Bitstream();
            ~Bitstream();

            void Insert(byte val);

            std::vector<byte> mStream;
            
    };

    int CountBits(byte val);
    void DumpBits(byte val);
    void DumpBits(word val);
    void DumpBits(dword val);
    void DumpBits(qword val);
}

#endif // _VORBIS_BITPACKING_H
