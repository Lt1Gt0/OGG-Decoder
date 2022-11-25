#include "vorbis/bitpacking.h"
#include <stdio.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <bits/stdc++.h>
#include <string.h>
#include <cstdint>
#include <type_traits>

int32_t twosComplement32(uint32_t data)
{
     return(static_cast<int32_t>(data));
}

uint32_t invsComplement32(int32_t data)
{
     return(static_cast<uint32_t>(data));
}

int16_t twosComplement16(uint16_t data)
{
     return(static_cast<int16_t>(data));
}

uint16_t invsComplement16(int16_t data)
{
     return(static_cast<uint16_t>(data));
}

int8_t twosComplement8(uint8_t data)
{
     return(static_cast<int8_t>(data));
}

uint8_t invsComplement8(int8_t data)
{
     return(static_cast<uint8_t>(data));
}

namespace Vorbis
{
    Bitstream::Bitstream()
    {
        mStream = std::vector<char*>(); 
        mBitCursor = 0;
    }

    Bitstream::~Bitstream()
    {

    }

    void Bitstream::Insert(uint32_t _val) 
    {
        auto EmptyByteString = []() -> char* 
        {
            char* buf = new byteString; 
            memset(buf, '\0', sizeof(byteString));
            return buf;
        };

        if (mStream.size() == 0)
            mStream.push_back(EmptyByteString());

        char* streamVal = mStream.back();

        for (char c : DumpBits(_val)) {
            int bit = (c == '0') ? 0 : 1;

            if (mBitCursor == 7) {
                mStream.push_back(streamVal);
                mBitCursor = 0;
                streamVal = EmptyByteString();
            }    

            ModifyBit(streamVal, mBitCursor, bit);
            mBitCursor++;
        }
    }
            
    //void Bitstream::ModifyBit(byte* val, byte offset)
    void Bitstream::ModifyBit(char* buf, int offset, bool set)
    {
        buf[offset] = set ? '1' : '0';
        //offset = pow(2, offset - 1);
        //*val ^= offset;
    }

    //void Bitstream::ModifyBit(byte* val, byte offset, bool set)
    //{
        //// TODO
    //}
}
