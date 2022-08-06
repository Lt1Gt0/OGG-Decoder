#pragma once
#ifndef _VORBIS_BITPACKING_H
#define _VORBIS_BITPACKING_H

#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <exception>

typedef uint8_t byte;
typedef byte octet;
typedef uint16_t word;
typedef uint32_t dword;

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
        
        // Encoder function
        void        Write(uint32_t data); 
        
        // Encoder/Decoder Functions?
        byte        ReadByte();
        byte        ReadByte(size_t byteOffset, uint8_t bitCursor);
        word        ReadWord();
        word        ReadWord(size_t byteOffset, uint8_t bitCursor);
        dword       ReadDword(size_t bitCount);   
        dword       ReadDword(size_t byteOffset, uint8_t bitCursor, size_t bitCount);   
    private:
        bool ReadBit(const dword& input, dword offset);

        /* ---------- EXCEPTIONS ---------- */  
        class OutOfRange : public std::exception
        {
            virtual const char* what() const throw(); 
        } out_of_range;
};

#endif // _VORBIS_BITPACKING_H
