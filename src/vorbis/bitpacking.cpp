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
    /* TODO */
}

word ByteStream::ReadWord()
{
    return ReadWord(this->mByteOffset, this->mBitCursor); 
}

word ByteStream::ReadWord(size_t _byteOffset, uint8_t _bitCursor)
{
    /* TODO */
}

dword ByteStream::ReadDword(size_t _bitCount)
{
    return ReadDword(this->mByteOffset, this->mBitCursor, _bitCount);
}

dword ByteStream::ReadDword(size_t _byteOffset, uint8_t _bitCursor, size_t _bitCount)
{
    uint32_t data;
    
    for (size_t i = 0; i < _bitCount; i++) {
        if (this->mBitCursor == 8) {
            this->mByteOffset++;  
        } 
    }

    return data;
}

void ByteStream::SetBit(byte _byte, uint8_t _pos, uint8_t _val)
{
    
}

uint8_t ByteStream::ReadBit(byte _byte, uint8_t _pos)
{
    // TODO
    return _byte >> _pos;
}

uint8_t ByteStream::ReadBit()
{
    return ReadBit(this->mByteOffset, this->mBitCursor);
}

/* ---------- EXCEPTIONS ---------- */  
const char* ByteStream::OutOfRange::what() const throw()
{
    return "Out of range";
} 
