#pragma once
#ifndef _OGG_VORBIS_APPLICATION_H
#define _OGG_VORBIS_APPLICATION_H

#include "oggmeta.h"

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
    #define VORBIS_OCTET            "vorbis"
    #define VORBIS_OCTET_LENGTH     6 
    #define INVALID_VORBIS_VERSION  -1

    #define NULL_COMMENT            {0, nullptr}

    #define CODEBOOK_SYNC_PATTERN   0x56, 0x43, 0x42

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
        uint8_t Packet;
        uint8_t Magic[6];            
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
        uint8_t     FramingFlag : 1;
        uint8_t     RESV        : 7;
    } __attribute__((packed));

    // There are different versions of vorbis applications according to the spec
    // Currently I don't how how I am going to handle them

    struct Comment {
        uint32_t    Length;
        uint8_t*    UserComment; 
    };

    struct CommentsHeader {
        uint32_t VendorLength;
        uint8_t* VendorString;
        uint32_t UserCommentListLength;
        std::vector<Comment> comments;
        uint8_t FramingBit; // Read single bit as boolean
    };

    struct Codebook {
        uint8_t syncPattern[3]; 

    };
    
    struct SetupHeader {
        // List of codebook configurations
        uint8_t codebookCount;
        Codebook* codebookConfigurations; 

        // Time-domain transform configurations (placeholders in Vorbis I)
        // Floot configurations
        // Residue Configurations
        // Channel mapping configurations
        // mode configurations
        // Framing bit
    } __attribute__((packed));

    struct Bitstream {
        IdentificationHeader identification;
        CommentsHeader comments;
        SetupHeader setup;    
    };

    struct Application {
        CommonHeader common;
        Bitstream bitstream;
    };

    void CheckCodec(FILE* fp, OggCodec* ret, int codec);

    int LoadPacket(FILE* fp);
    IdentificationHeader* LoadIdentificationHeader(FILE* _fp);
    CommentsHeader* LoadCommentsHeader(FILE* _fp);
    SetupHeader* LoadSetupHeader(FILE* _fp);

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

    /* ---------- EXCEPTIONS ---------- */   
    static class InvalidPacketType : public std::exception
    {
        virtual const char* what() const throw(); 
    } invalid_packet_type;

    static class InvalidAudioChannels : public std::exception
    {
        virtual const char* what() const throw();
    } invalid_audio_channels;

    static class InvalidAudioSampleRate : public std::exception
    {
        virtual const char* what() const throw();
    } invalid_audio_sample_rate;

    static class InvalidBlockSize : public std::exception
    {
        virtual const char* what() const throw(); 
    } invalid_block_size;

    static class FramingBitNotSet : public std::exception
    {
        virtual const char* what() const throw(); 
    } framing_bit_not_set;
    
    static class EndOfPacket : public std::exception
    {
        virtual const char* what() const throw(); 
    } end_of_packet;
}

#endif // _OGG_VORBIS_APPLICATION_H
