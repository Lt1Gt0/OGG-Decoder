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
        mStream = std::vector<BitPattern>(); 
        mBitCursor = 0;
    }

    Bitstream::~Bitstream()
    {

    }

    void Bitstream::Insert(uint32_t _val) 
    {
        auto EmptBitPattern = []() -> BitPattern 
        {
            BitPattern pattern; 
            pattern.mBuf = new char[8];
            memset(pattern.mBuf, '0', sizeof(char) * 8);
            return pattern;
        };

        if (mStream.size() == 0)
            mStream.push_back(EmptBitPattern());

        BitPattern streamVal = mStream.back();
        mStream.pop_back();

        for (char c : DumpBits(_val)) {
            if (mBitCursor == 7) {
                mStream.push_back(streamVal);
                mBitCursor = 0;
                streamVal = EmptBitPattern();
            }    

            streamVal.mBuf[mBitCursor] = c;
            mBitCursor++;
        }

        mStream.push_back(streamVal);
    }

    //void Bitstream::ModifyBit(byte* val, byte offset, bool set)
    //{
        //// TODO
    //}
}
