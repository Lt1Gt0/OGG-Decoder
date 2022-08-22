#include "vorbis/bitpacking.h"
#include <stdio.h>
#include <vector>

namespace Vorbis
{
    Bitstream::Bitstream()
    {
        mStream = std::vector<byte>(); 
    }

    Bitstream::~Bitstream()
    {

    }

    int CountBits(byte val)
    {
        int bits = 0;
        while (val) {
            bits++;
            val >>= 1;
        }

        return bits;
    }

    void DumpBits(byte val)
    {
        byte bitCount = CountBits(val);
        for (int i = 0; i <= bitCount; i++) {
            if ((i - 1) % 4 == 0)
                printf(" "); 

            printf("%d", (val >> (bitCount - i)) & 0x1);
        }
    }

    void DumpBits(word val)
    {

    }

    void DumpBits(dword val)
    {

    }

    void DumpBits(qword val)
    {

    }
}
