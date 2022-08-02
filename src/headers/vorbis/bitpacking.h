#pragma once
#ifndef _VORBIS_BITPACKING_H
#define _VORBIS_BITPACKING_H

#include <stdint.h>
#include <stddef.h>

typedef uint8_t byte;
typedef byte octet;

struct ByteStream {
    byte*   stream;
    size_t  streamLength;
    uint8_t bitCursor;

    void    Write(size_t data); 
    size_t  Read(size_t byteOffset, uint8_t bitCursor);   
};

#endif // _VORBIS_BITPACKING_H
