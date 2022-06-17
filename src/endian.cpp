#include "endian.h"

namespace Endian
{
    uint16_t Reverse16(uint16_t value)
    {
        return (((value & 0x00FF) << 8) |
                ((value & 0xFF00) >> 8));
    }

    uint32_t Reverse32(uint32_t value)
    {
        return (((value & 0x000000FF) << 24) |
                ((value & 0x0000FF00) <<  8) |
                ((value & 0x00FF0000) >>  8) |
                ((value & 0xFF000000) >> 24));
    }

    uint64_t Reverse64(uint64_t value)
    {
        return (((value & 0x00000000000000FF) << 56) |
                ((value & 0x000000000000FF00) << 40) |
                ((value & 0x0000000000FF0000) << 24) |
                ((value & 0x00000000FF000000) <<  8) |
                ((value & 0x000000FF00000000) >>  8) |
                ((value & 0x0000FF0000000000) >> 24) |
                ((value & 0x00FF000000000000) >> 40) |
                ((value & 0xFF00000000000000) >> 56));
    }

    uint16_t LittleEndian16(uint16_t value)
    {
        if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
            return value;
        else if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
            return Reverse16(value);
        else
            return 0;
    }

    uint16_t BigEndian16(uint16_t value)
    {
        if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
            return value;
        else if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
            return Reverse16(value);
        else
            return 0;
    }

    uint32_t LittleEndian32(uint32_t value)
    {
        if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
            return value;
        else if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
            return Reverse32(value);
        else
            return 0;
    }

    uint32_t BigEndian32(uint32_t value)
    {
        if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
            return value;
        else if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
            return Reverse32(value);
        else
            return 0;
    }
    
    uint64_t LittleEndian64(uint64_t value)
    {
        if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
            return value;
        else if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
            return Reverse64(value);
        else
            return 0;
    }

    uint64_t BigEndian64(uint64_t value)
    {
        if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
            return value;
        else if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
            return Reverse64(value);
        else
            return 0;
    }
}