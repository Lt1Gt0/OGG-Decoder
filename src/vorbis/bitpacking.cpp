#include "vorbis/bitpacking.h"
#include <stdio.h>
#include <vector>
#include <math.h>

namespace Vorbis
{
    Bitstream::Bitstream()
    {
        mStream = std::vector<byte>(); 
    }

    Bitstream::~Bitstream()
    {

    }

    void Bitstream::Insert(byte val)
    {
        // TODO
        
        if (this->mStream.size() == 0)
            this->mStream.push_back(0);

        byte streamVal = this->mStream.back();
        for (int i = 0; i <= Vorbis::CountBits(val); i++) {
            if (this->mBitCursor == 7) {
                this->mStream.push_back(streamVal);
                this->mBitCursor = 0;
                streamVal = 0;
            }    

            ModifyBit(&streamVal, this->mBitCursor, (val >> (CountBits(val) - i) & 0x1));
            this->mBitCursor++;
        }
    }
            
    void Bitstream::ModifyBit(byte* val, byte offset)
    {
        offset = pow(2, offset - 1);
        *val ^= offset;
    }

    void Bitstream::ModifyBit(byte* val, byte offset, bool set)
    {
        // TODO
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

        printf("\n");
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
