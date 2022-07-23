#include "vorbis.h"
#include "Debug/logger.h"
#include <tgmath.h>

namespace Vorbis
{
    IdentificationHeader* LoadIdentificationHeader(FILE* _fp)
    {
        LOG_INFO << "Loading Identification Header" << std::endl;
        IdentificationHeader* identification= new IdentificationHeader;
        fread(identification, sizeof(uint8_t), sizeof(IdentificationHeader), _fp);
    
        if (identification->VorbisVersion != 0) {
            LOG_ERROR << "Vorbis Version: " << (int)identification->VorbisVersion << std::endl;
            return nullptr;
        } 

        if (identification->AudioChannels <= 0) {
            LOG_ERROR << "Audio Channels: " << (int)identification->AudioChannels << std::endl;
            throw invalid_audio_channels;
        }

        if (identification->AudioSampleRate <= 0) {
            LOG_ERROR << "Audio Sample Rate: " << (int)identification->AudioChannels << std::endl;
            throw invalid_audio_sample_rate;
        }

        if (!validBlockSize(identification->BlockSize)) {
            LOG_ERROR << "Block Size 0: " << (int)std::pow(2, identification->BlockSize & 0xF) << std::endl;
            LOG_ERROR << "Block Size 1: " << (int)std::pow(2, identification->BlockSize >> 4 & 0xF) << std::endl;
            throw invalid_block_size;
        }

        int bitstreamType = CheckBitstreamType(identification->BitrateMax, identification->BitrateNominal, identification->BitrateMin); 
        LOG_DEBUG << "Bitstream Type: " << (int)bitstreamType << std::endl;
        
        LOG_SUCCESS << "Loaded Identification Header" << std::endl;
        return identification;
    }

    CommentsHeader* LoadCommentsHeader(FILE* _fp)
    {
        LOG_INFO << "Loading Comments Header" << std::endl;
        CommentsHeader* commentsHeader = new CommentsHeader;
        fread(commentsHeader, sizeof(uint8_t), sizeof(CommentsHeader), _fp);
        
        LOG_SUCCESS << "Loaded Comments Header" << std::endl;
        return commentsHeader; 
    }

    SetupHeader* LoadSetupHeader(FILE* _fp)
    {
        LOG_INFO << "Loading Setup Header" << std::endl;
        SetupHeader* setupHeader = new SetupHeader;
        fread(setupHeader, sizeof(uint8_t), sizeof(SetupHeader), _fp);
        
        LOG_SUCCESS << "Loaded Setup Header" << std::endl;
        return setupHeader; 
    }
    
    bool validBlockSize(uint8_t block)
    {
        /*
         * Vorbis 1 specs show that blockSize0 is the lower 4 bits of blocksize
         * and blockSize1 is the upper 4 bits, both blockSizes are then truely
         * calculated by setting them to 2^blockSize_n
         */
        int blockSize0 = std::pow(2, block & 0xF);
        int blockSize1 = std::pow(2, block >> 4 & 0xF);
        bool validBlock0 = false;
        bool validBlock1 = false;
        constexpr int VALID_BLOCK_SIZE_COUNT = 8;
        int validBlockSizes[VALID_BLOCK_SIZE_COUNT] = {64, 128, 256, 512, 1024, 2048, 4096, 8192};

        // Check if any of the block sized are not valid
        for (int i = 0; i < VALID_BLOCK_SIZE_COUNT; i++) {
            if (blockSize0 == validBlockSizes[i])
                validBlock0 = true;

            if (blockSize1 == validBlockSizes[i])
                validBlock1 = true;     
        } 

        // Case in which oneor both block sizes are invalid
        if (!(validBlock0 && validBlock1))
            return false;

        // Final Check according to vorbis spec
        return blockSize0 <= blockSize1; 
    }

    int CheckBitstreamType(uint32_t max, uint32_t nom, uint32_t min)
    {
        // Given in vorbis spec pg 27
        
        // max == nom == min --> fixed rate/tightly bounded/nearly fixed-rate 
        if (max == nom && max == min)
            return (int)BitstreamType::TripleSet; 
        
        // only nominal --> VBR/ABR 
        if (nom && (!min && !max))
            return (int)BitstreamType::NominalOnly; 

        // min || min --> VBR
        if (max || min)
            return (int)BitstreamType::VBR;

        return (int)BitstreamType::None; 
    }

    /* ---------- EXCEPTIONS ---------- */   
    const char* InvalidPacketType::what() const throw()
    {
        return "Invalid Packet Type"; 
    }

    const char* InvalidAudioChannels::what() const throw()
    {
        return "Invalid Audio Channel"; 
    }

    const char* InvalidAudioSampleRate::what() const throw()
    {
        return "Invalid Audio Sample Rate"; 
    } 

    const char* InvalidBlockSize::what() const throw()
    {
        return "Invalid Block Size"; 
    } 
}
