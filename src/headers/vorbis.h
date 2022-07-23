#pragma once
#ifndef _OGG_VORBIS_APPLICATION_H
#define _OGG_VORBIS_APPLICATION_H

#include "oggmeta.h"

#include <stdint.h>
#include <exception>
#include <stdio.h>

/*
Steps to decode (according to Vorbis I Spec)
- Decode packet type flag
- Decode mode number
- Decode window shape (long windows only)
- Decode floor
- Decode residue into residue vectors
- Inverse channel coupling of residue vectors
- Generate floot curve from decoded floor data
- Compute dot product of floor and residue, producing audio spectrum vector
- Inverse monolithic transform of audio spectrum vector, always an MDCT in Vorbis I
- Overlap/add left-hand output of transform with right-hand output of previous frame
- Store right-hand data from transform of current frame for further lapping
- If not the first frame, return results of overlap/add as audio result of current frame
*/

namespace Vorbis {
    #define VORBIS_OCTET "vorbis"

    #define INVALID_VORBIS_VERSION -1

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
    // Currently I dont how how I am going to handle them

    struct CommentsHeader {
        uint32_t VendorLength;
        // Vendor String
        uint32_t UserCommentListLength;

    };
    
    struct SetupHeader {
        uint8_t CodebookCount;
        //std::vector<Codebook> CodebookConfigurations;
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

    struct Codebook {
        uint8_t Count;            
    } __attribute__((packed));


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
    int CheckBitstreamType(uint32_t max, uint32_t nom, uint32_t min);

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
}

#endif // _OGG_VORBIS_APPLICATION_H
