#pragma once
#ifndef ENDIAN_H
#define ENDIAN_H

#include <stdint.h>

namespace Endian
{
    uint16_t Reverse16(uint16_t value);
    uint32_t Reverse32(uint32_t value);
    uint64_t Reverse64(uint64_t value);

    uint16_t LittleEndian16(uint16_t value);
    uint16_t BigEndian16(uint16_t value);

    uint32_t LittleEndian32(uint32_t value);
    uint32_t BigEndian32(uint32_t value);
    
    uint64_t LittleEndian64(uint64_t value);
    uint64_t BigEndian64(uint64_t value);
}

#endif // ENDIAN_H