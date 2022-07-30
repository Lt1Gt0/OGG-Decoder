#include "vorbis/codebook.h"
#include "vorbis/vorbis.h"
#include "Debug/logger.h"

namespace Codebooks 
{
    /**
     * return the amount of bytes that will be read from a file
     * as well as gif the offset of bits needed to be read from
     * the final byte
     *
     * (bytes and bits will be modified on return)
     *
     * @param bitCount - total number of bits
     * @param bytes - number of bytes caclutated from bitCount
     * @param bits - number of extra bits 
     */
    void BitsAndBytes(int bitCount, int* bytes, int* bits)
    {
        *bytes = bitCount / 8; 
        *bits = *bytes % 8;
    }

    void LoadCodebooks(FILE* fp, Codebook* codebookConfigurations, uint8_t codebookCount)
    {
        LOG_DEBUG << "Loading [" << (int)codebookCount << "] codebooks" << std::endl; 
        for (int i = 0; i < codebookCount; i++) {
            Codebook* codebook = new Codebook; 
           
            // Load the header data for the codebook (not including the flag byte)
            fread(&codebook->raw, sizeof(uint64_t), 1, fp); 
            if (!VerifyCodebook(*codebook))
                error(Severity::medium, "Vorbis Codebook:", "before crash -", "file pos =", ftell(fp));

            // Read flag byte
            fread(&codebook->Ordered, sizeof(uint8_t), 1, fp);

            codebook->Entries = new Entry[codebook->EntryCount]; 
            int entryIdx; 

            for (entryIdx = 0; entryIdx < codebook->EntryCount; entryIdx++) {
                if (!(codebook->Ordered & 0x1)) { // Ordered Flag is unset
                    uint8_t sparse = (codebook->Ordered >> 1 & 0x1);
                    if (sparse) {
                        uint8_t flag = (codebook->Ordered >> 2) & 0x1;
                        if (flag) {
                            uint8_t length = (codebook->Ordered >> 3) & 0x5;
                            codebook->Entries[entryIdx].codewordLength = length + 1;
                            codebook->Entries[entryIdx].unused = false; // Just incase
                        } else {
                            // This entry will be unused so mark it
                            codebook->Entries[entryIdx].unused = true; 
                        } 
                    } else { // Ordered Flag is set
                        uint8_t length = (codebook->Ordered >> 2) & 0x5;
                        codebook->Entries[entryIdx].codewordLength = length + 1;
                        codebook->Entries[entryIdx].unused = false; // Just incase
                    }
                } else { // Ordered Flag is set
                    int bitCount;
                    int byteCount;
                    int number;
                    int currentEntry = 0;
                    uint8_t currentLegth = ((codebook->Ordered >> 1) & 0x5) + 1;
                    
                    while (currentEntry < codebook->EntryCount) {
                        // Read [ilog(entrycount - currentEntry)] bits from the file int [number]
                        BitsAndBytes(ilog(codebook->EntryCount - currentEntry), &byteCount, &bitCount);
                        fread(&number, sizeof(uint8_t), byteCount, fp);
                        if (bitCount > 0) {
                            uint8_t nextByte;
                            fread(&nextByte, sizeof(uint8_t), 1, fp);
                            number += nextByte & bitCount; 
                        }

                        for (int entryNum = currentEntry; entryNum <= currentEntry + number - 1; entryNum++) {
                            codebook->Entries[entryNum].codewordLength = currentLegth; 
                        }

                        currentEntry += number;
                        currentLegth++;

                        if (currentEntry > codebook->EntryCount)
                            error(Severity::high, "Vorbis Codebook:", "Current Entry is greater than codebook entries");
                    }
                }
            }

            // Load the codebook data into the configuration list
            codebookConfigurations[i] = *codebook;
        }
    }

    int VerifyCodebook(const Codebook& codebook)
    {
        // Iterate [codebook_sync_pattern] bytes
        for (int i = 0; i < 3; i++) {
            uint8_t syncByte = (codebook.raw >> ((int)RawOffsets::syncPattern + (i * 8))) & 0xFF;
            //printf("syncByte: 0x%X - CodebookSyncPattern: 0x%X\n", syncByte, CODEBOOK_SYNC_PATTERN[i]);
            if (syncByte != CODEBOOK_SYNC_PATTERN[i])
               return 0; 
        }

        LOG_SUCCESS << "Verified Codebook" << std::endl;
        return 1; 
    }
}
