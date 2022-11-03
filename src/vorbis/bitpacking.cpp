#include "vorbis/bitpacking.h"
#include <stdio.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <bits/stdc++.h>
#include <string.h>

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

    void Bitstream::Insert(byte _val)
    {
        auto EmptyByteString = []() -> char* {
            char* buf = new char[BITS_IN_BYTE]; 
            memset(buf, '0', BITS_IN_BYTE);
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
    void Bitstream::ModifyBit(char* buf, byte offset, bool set)
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
