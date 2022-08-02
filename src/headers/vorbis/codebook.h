#pragma once
#ifndef VORBIS_CODEBOOK_H
#define VORBIS_CODEBOOK_H

#include <stdio.h>
#include <stdint.h>
#include <vector>

#include "vorbis/bitpacking.h"

namespace Codebooks
{
    #define NULL_CODEBOOK {0, 0, nullptr}
    constexpr byte CODEBOOK_SYNC_PATTERN[3] = {0x42, 0x43, 0x56};

    enum class RawOffsets {
        syncPattern = 0,
        dimensions  = 24,
        entryCount  = 40, 
    };

    struct Entry {
        int     codewordLength;
        bool    unused; 
    };

    struct Codebook {
        union {
            struct {
                uint64_t SyncPattern    : 24;
                uint64_t Dimensions     : 16;
                uint64_t EntryCount     : 24;
            };

            uint64_t raw;
        };

        uint8_t Ordered;
        Entry*  Entries;
    };

    Codebook* LoadCodebooks(FILE* fp, uint8_t codebookCount);
    int VerifyCodebook(const Codebook& codebook);
}

#endif // VORBIS_CODEBOOK_H
