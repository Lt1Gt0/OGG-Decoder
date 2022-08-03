#include "vorbis/bitpacking.h"

ByteStream::ByteStream()
{
    this->mStream = std::vector<byte>();
    this->mBitCursor = 0;
    this->mByteOffset = 0;
}

ByteStream::~ByteStream()
{

}

/* This method should mainly be used if I was working
 * on an encoder so I will leave it here as is */
void ByteStream::Write(uint32_t _data)
{

}

byte ByteStream::ReadByte()
{
    return ReadByte(this->mByteOffset, this->mBitCursor); 
}

byte ByteStream::ReadByte(size_t _byteOffset, uint8_t _bitCursor)
{
    this->mByteOffset = _byteOffset;
    this->mBitCursor = _bitCursor;

    /* TODO */
}

uint16_t ByteStream::ReadWord()
{
    return ReadWord(this->mByteOffset, this->mBitCursor); 
}

uint16_t ByteStream::ReadWord(size_t _byteOffset, uint8_t _bitCursor)
{
    this->mByteOffset = _byteOffset;
    this->mBitCursor = _bitCursor;

    /* TODO */
}

uint32_t ByteStream::Read(size_t _bitCount)
{
    uint32_t data;
    
    for (size_t i = 0; i < _bitCount; i++) {
        if (this->mBitCursor == 8) {
            this->mByteOffset++;  
        } 
    }

    return data;
}

uint32_t ByteStream::Read(size_t _byteOffset, uint8_t _bitCursor, size_t _bitCount)
{
    this->mBitCursor = _bitCursor;
    this->mByteOffset = _byteOffset;
    return Read(_bitCount);
}

void SetBit(byte _byte, uint8_t _pos, uint8_t _val)
{
    
}

/* ---------- EXCEPTIONS ---------- */  
const char* ByteStream::OutOfRange::what() const throw()
{
    return "Out of range";
} 
