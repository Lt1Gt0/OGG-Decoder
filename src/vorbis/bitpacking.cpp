#include "vorbis/bitpacking.h"

ByteStream::ByteStream() : 
    mStream(std::vector<byte>()), mBitCursor(0), mByteOffset(0)
{

}
ByteStream::~ByteStream() {}

/* This method should mainly be used if I was working
 * on an encoder so I will leave it here as is */
void ByteStream::Write(uint32_t _data)
{
    return;
}

byte ByteStream::ReadByte()
{
    return ReadByte(this->mByteOffset, this->mBitCursor); 
}

byte ByteStream::ReadByte(size_t _byteOffset, uint8_t _bitCursor)
{
    byte ret = 0;

    if (_bitCursor == 0)
        return this->mStream.at(_byteOffset);
    

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

bool ByteStream::ReadBit(const dword& input, dword offset)
{
    return input >> offset;
}

/* ---------- EXCEPTIONS ---------- */  
const char* ByteStream::OutOfRange::what() const throw()
{
    return "Out of range";
} 
