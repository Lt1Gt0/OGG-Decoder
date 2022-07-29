#pragma once
#ifndef VORBIS_CODEBOOK_H
#define VORBIS_CODEBOOK_H

#include <stdio.h>
#include <stdint.h>
#include <vector>

#include "common.h"

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

    void LoadCodebooks(FILE* fp, Codebook* codebookConfigurations, uint8_t codebookCount);
    int VerifyCodebook(const Codebook& codebook);
    
    /* ---------- EXCEPTIONS ---------- */   
    static class InvalidCodebook : public std::exception
    {
        virtual const char* what() const throw(); 
    } invalid_codebook;

    static class InvalidEntryAmount: public std::exception
    {
        virtual const char* what() const throw(); 
    } invalid_entry_amount;
}

#endif // VORBIS_CODEBOOK_H
