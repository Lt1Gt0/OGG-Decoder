#pragma once
#ifndef _OGG_VORBIS_META_H
#define _OGG_VORBIS_META_H

#include "oggmeta.h"

#include <stdint.h>

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

namespace OggMeta {
    namespace Vorbis {
        #define VORBIS_OCTET "vorbis"

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
            uint8_t     BlockSize0 : 4;
            uint8_t     BlockSize1 : 4;

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
        
        struct Codebook {
            uint8_t Count;            
        } __attribute__((packed));

        struct SetupHeader {
            uint8_t CodebookCount;
            //std::vector<Codebook> CodebookConfigurations;

            
        } __attribute__((packed));



        // struct 

    } // namespace Vorbis
} // namespace OggMeta

#endif // _OGG_VORBIS_META_H
