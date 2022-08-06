#pragma once
#ifndef _OGG_VORBIS_APPLICATION_H
#define _OGG_VORBIS_APPLICATION_H

#include "oggmeta.h"
#include "vorbis/codebook.h"
#include "common.h"

#include <stdint.h>
#include <exception>
#include <stdio.h>
#include <vector>

/*
Steps to decode (according to Vorbis I Spec)
- Decode packet type flag
- Decode mode number
- Decode window shape (long windows only)
- Decode floor
- Decode residue into residue vectors
- Inverse channel coupling of residue vectors
- Generate floor curve from decoded floor data
- Compute dot product of floor and residue, producing audio spectrum vector
- Inverse monolithic transform of audio spectrum vector, always an MDCT in Vorbis I
- Overlap/add left-hand output of transform with right-hand output of previous frame
- Store right-hand data from transform of current frame for further lapping
- If not the first frame, return results of overlap/add as audio result of current frame
*/

namespace Vorbis {
    constexpr int VORBIS_OCTET_LENGTH                       {6};
    constexpr octet VORBIS_OCTET[VORBIS_OCTET_LENGTH]       {'v', 'o', 'r', 'b', 'i', 's'};
    constexpr int INVALID_VORBIS_VERSION                    {-1};
    constexpr int VALID_BLOCK_SIZE_COUNT                    {8};
    constexpr int validBlockSizes[VALID_BLOCK_SIZE_COUNT]   {64, 128, 256, 512, 1024, 2048, 4096, 8192};

    enum class PacketType : uint8_t {
        Identification  = 0x01,
        Comment         = 0x03,
        Setup           = 0x05, 
    };

    /*
     * TripleSet -> Hint to the decoder later that the bistream type is either
     * a fixed rate, tightly bounded or a nearly fixed rate
     *
     * NominalOnly -> Hint to decoder later that the bitstream type is either
     * VBR or ABR
     */
    enum class BitstreamType : int {
        TripleSet,
        FixedRate, 
        TightlyBounded,
        NearlyFixedRate,
        NominalOnly,
        VBR,
        ABR,
        None,
    };

    struct CommonHeader {
        byte    Packet;
        octet   Magic[6];            
    } __attribute__((packed));

    struct IdentificationHeader {
        uint32_t    VorbisVersion;
        uint8_t     AudioChannels;
        uint32_t    AudioSampleRate;
        uint32_t    BitrateMax;
        uint32_t    BitrateNominal;
        uint32_t    BitrateMin;
        
        // Block size 0 = lower 4 bits
        // Block size 1 = upper 4 bits 
        uint8_t     BlockSize;

        // Framing flag says to only read 1 bit so I guess the other 7 are RESV
        uint8_t     FramingFlag;
    } __attribute__((packed));

    // There are different versions of vorbis applications according to the spec
    // Currently I don't how how I am going to handle them

    /* The data in the comments are octect aligned
     * hence the reason why I use a(ligned)Octet */
    struct Comment {
        uint32_t    Length;
        octet*      UserComment; 
    } __attribute__((aligned(32)));

    struct CommentsHeader {
        uint32_t                VendorLength;
        octet*                  VendorString;
        uint32_t                UserCommentListLength;
        std::vector<Comment>    comments;
        octet                   FramingBit; // Read single bit as boolean
    } __attribute__((aligned(32)));

    
    struct SetupHeader {
        // List of codebook configurations
        uint8_t                 codebookCount;
        Codebooks::Codebook*    codebookConfigurations; 

        // Time-domain transform configurations (placeholders in Vorbis I)
        // Floor configurations
        // Residue Configurations
        // Channel mapping configurations
        // mode configurations
        // Framing bit
    } __attribute__((packed));

    struct Bitstream {
        IdentificationHeader    identification;
        CommentsHeader          comments;
        SetupHeader             setup;    
    };

    struct Application {
        CommonHeader    common;
        Bitstream       bitstream;
    };

    /**
     * Check if the current position at a given file contains
     * the codec signature for a vorbis mapping
     * 
     * @param fp - file input
     * @param ret - OccCodec modifier, if the file contains the vorbis mapping
     * signature the value will be changed to [codec], if not then it will 
     * be set to OggCodec::Unknown
     * @param codec - codec integer found in OggCodec enum 
     */
    OggCodec CheckCodec(FILE* fp, int codec);

    int LoadPacket(FILE* fp);
    void LoadIdentificationHeader(FILE* fp, IdentificationHeader* identification);
    void LoadCommentsHeader(FILE* fp, CommentsHeader* comments);
    void LoadSetupHeader(FILE* fp, SetupHeader* setup);

    /**
     * Check to see if the blocksize inside of identification header
     * is a valid type according to vorbis I spec
     *
     * @param blockSize - Identification header block size
     */
    bool validBlockSize(uint8_t blocksize);

    /**
     * Determine the type of bitstream given from the Identification Header
     *
     * @param max - Bitrate maximum
     * @param nom - Bitrate nominal
     * @param min - Bitrate minimum
     */
    BitstreamType CheckBitstreamType(uint32_t max, uint32_t nom, uint32_t min);
}

#endif // _OGG_VORBIS_APPLICATION_H
