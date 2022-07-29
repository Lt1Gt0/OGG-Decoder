#include "vorbis/vorbis.h"
#include "oggmeta.h"
#include "Debug/logger.h"
#include "common.h"

#include <tgmath.h>
#include <string.h>

namespace Vorbis
{
    void CheckCodec(FILE* fp, OggCodec* ret, int codec)
    {
        using namespace OggMeta;

        // Store the current position of the file
        fpos_t pos;
        fgetpos(fp, &pos);

        Vorbis::CommonHeader* common= new CommonHeader;
        fread(common, sizeof(uint8_t), sizeof(CommonHeader), fp);

        // Set the file position to a previous state and delete common header
        auto clean = [&, fp, pos, common] () mutable -> void
        {
            fsetpos(fp, &pos);
            delete common;
            common = NULL; 
        };

        // If the vorbis ocetet does not match, make the return value Unknown
        for (int i = 0; i < VORBIS_OCTET_LENGTH; i++) {
            // common is not vorbis 
            if (common->Magic[i] != VORBIS_OCTET[i]) {
                clean();
                *ret = OggCodec::Unknown;
                return;
            }
        }

        // Restore file to previous state
        clean();
        *ret = (OggCodec)codec;
    }

    int LoadPacket(FILE* fp)
    {
        using namespace OggMeta;
        using namespace Vorbis;

        CommonHeader* common = new CommonHeader;
        fread(common, sizeof(uint8_t), sizeof(CommonHeader), fp);

        switch ((PacketType)common->Packet) {
            case PacketType::Identification:
            {
                IdentificationHeader* identification = LoadIdentificationHeader(fp);

                if (identification == NULL)
                    return INVALID_VORBIS_VERSION; 

                break;
            } 
            case PacketType::Comment:
            {
                LoadCommentsHeader(fp);

                break;
            } 
            case PacketType::Setup:
            {
                LoadSetupHeader(fp);
                break;
            }
            default:
            {
                LOG_ERROR << "Invalid Packet Type: " << (int)common->Packet << std::endl; 
                throw invalid_packet_type; 
            }
        }

        // If the next segment in the file contains a vorbis packet signature load again
        OggCodec codec = OggCodec::Unknown;
        CheckCodec(fp, &codec, (int)OggCodec::Vorbis);
        if (codec == OggCodec::Vorbis)
            LoadPacket(fp);

        return 1;
    }

    IdentificationHeader* LoadIdentificationHeader(FILE* fp)
    {
        LOG_INFO << "Loading Identification Header" << std::endl;
        IdentificationHeader* identification = new IdentificationHeader;
        fread(identification, sizeof(uint8_t), sizeof(IdentificationHeader), fp);
    
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

        BitstreamType bitstreamType = CheckBitstreamType(identification->BitrateMax, identification->BitrateNominal, identification->BitrateMin); 
        LOG_DEBUG << "Bitstream Type: " << (int)bitstreamType << std::endl;
        
        LOG_SUCCESS << "Loaded Identification Header" << std::endl;
        return identification;
    }

    CommentsHeader* LoadCommentsHeader(FILE* fp)
    {
        // Vector lengths and number of vectors are stored LSB first
        // Data in comment header is OCTET aligned

        LOG_INFO << "Loading Comments Header" << std::endl;
        CommentsHeader* comments = new CommentsHeader;
        
        // Load the vendor information 
        fread(&comments->VendorLength, sizeof(uint32_t), 1, fp);
        comments->VendorString = new octet[comments->VendorLength];
        fread(&comments->VendorString, sizeof(octet), comments->VendorLength, fp);
        fread(&comments->UserCommentListLength, sizeof(uint32_t), 1, fp);
        
        for (uint32_t i = 0; i < comments->UserCommentListLength; i++) {
            Comment comment = NULL_COMMENT;
            fread(&comment.Length, sizeof(uint32_t), 1, fp);

            comment.UserComment = new octet[comment.Length];
            fread(comment.UserComment, sizeof(octet), comment.Length, fp);
            comments->comments.push_back(comment);  
        }

        /* uncomment this code block to see comment information for whatever reason 
        for (Comment com : comments->comments) {
            printf("Comment length: %d\n", com.Length);
            
            int i = 0;
            while (com.UserComment[i]){
                printf("%c", (char)com.UserComment[i]);
                i++;
            }

            printf("\n");
        }

        fflush(stdout); 
        */

        // Read Framing Bit and check it
        fread(&comments->FramingBit, sizeof(octet), 1, fp);

        if (!comments->FramingBit)
            throw framing_bit_not_set;

        // TODO
        // Check for end of packet
        // if (endOfPacket)
            // throw end_of_packet;
        
        LOG_SUCCESS << "Loaded Comments Header" << std::endl;
        return comments; 
    }

    SetupHeader* LoadSetupHeader(FILE* fp)
    {
        LOG_INFO << "Loading Setup Header" << std::endl;
        SetupHeader* setup = new SetupHeader;

        // Get the codbook count + 1
        fread(&setup->codebookCount, sizeof(uint8_t), 1, fp);
        setup->codebookCount++;

        // Allocate memory for codebookConfigurations and load each codebook
        setup->codebookConfigurations = new Codebooks::Codebook[setup->codebookCount];
        Codebooks::LoadCodebooks(fp, setup->codebookConfigurations, setup->codebookCount);

        LOG_SUCCESS << "Loaded Setup Header" << std::endl;
        return setup; 
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

        // Case in which one or both block sizes are invalid
        if (!(validBlock0 && validBlock1))
            return false;

        // Final Check according to vorbis spec
        return blockSize0 <= blockSize1; 
    }

    BitstreamType CheckBitstreamType(uint32_t max, uint32_t nom, uint32_t min)
    {
        // Given in vorbis spec pg 27
        
        // max == nom == min --> fixed rate/tightly bounded/nearly fixed-rate 
        if (max == nom && max == min)
            return BitstreamType::TripleSet; 
        
        // only nominal --> VBR/ABR 
        if (nom && (!min && !max))
            return BitstreamType::NominalOnly; 

        // min || min --> VBR
        if (max || min)
            return BitstreamType::VBR;

        return BitstreamType::None; 
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

    const char* FramingBitNotSet::what() const throw()
    {
        return "Framing Bit Not Set"; 
    } 

    const char* EndOfPacket::what() const throw()
    {
        return "End Of Packet"; 
    } 
}

int ilog(int x)
{
    int ret = 0;
    while (x > 0) {
        ret++;
        x >>= 1; 
    }

    return ret;
}