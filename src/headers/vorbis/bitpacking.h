#pragma once
#ifndef _VORBIS_BITPACKING_H
#define _VORBIS_BITPACKING_H

#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <exception>

typedef uint8_t byte;
typedef byte octet;

/* the maximum number of bits that are contained when writing to a byte
 * stream from the view of OGG is 32 bits */

// FIXME
// Signed and unsigned data need to be handled based off of context in the
// stream, which I do not know how to handle as of now meaning everything
// in the bytestream is treated as unsigned data
class ByteStream {
    public:
        std::vector<byte>   mStream;
    private:
        uint8_t             mBitCursor;
        size_t              mByteOffset;

    public:
        ByteStream();
        ~ByteStream();
        
        void        Write(uint32_t _data); 
        byte        ReadByte();
        byte        ReadByte(size_t _byteOffset, uint8_t _bitCursor);
        uint16_t    ReadWord();
        uint16_t    ReadWord(size_t _byteOffset, uint8_t _bitCursor);
        uint32_t    Read(size_t _bitCount);   
        uint32_t    Read(size_t _byteOffset, uint8_t _bitCursor, size_t _bitCount);   
    private:
        void SetBit(byte _byte, uint8_t _pos, uint8_t _val);

        /* ---------- EXCEPTIONS ---------- */  
        class OutOfRange : public std::exception
        {
            virtual const char* what() const throw(); 
        } out_of_range;
};

#endif // _VORBIS_BITPACKING_H
