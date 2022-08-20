#pragma once
#ifndef _VORBIS_BITPACKING_H
#define _VORBIS_BITPACKING_H

#include <stdint.h>
#include <vector>

#ifdef VORBIS_CODEC
    typedef uint8_t byte;
    typedef byte octet;
    typedef uint16_t word;
    typedef uint32_t dword;
    typedef uint64_t qword;
#endif // VORBIS_CODEC

namespace Vorbis
{
    class Bitstream
    {
           
    };
}

#endif // _VORBIS_BITPACKING_H
