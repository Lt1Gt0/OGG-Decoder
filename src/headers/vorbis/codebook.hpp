#pragma once
#include <cstdint>
#ifndef VORBIS_CODEBOOK_HPP_
#define VORBIS_CODEBOOK_HPP_

namespace Vorbis
{
    constexpr uint8_t CODEBOOK_SYNC_PATTERN[3] = {0x42, 0x43, 0x56};

    class Codebook
    {
        public:
            Codebook();
            ~Codebook();
    };
}

#endif // VORBIS_CODEBOOK_HPP_
