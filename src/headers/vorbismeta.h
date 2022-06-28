#pragma once
#ifndef _OGG_VORBIS_META_H
#define _OGG_VORBIS_META_H

#include "oggmeta.h"

#include <stdint.h>

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

        struct CommentsHeader {
            uint32_t VendorLength;
            // Vendor String
            uint32_t UserCommentListLength;

        };
        
        struct Codebook {
            
        } __attribute__((packed));

        struct SetupHeader {
            uint8_t CodebookCount;
            std::vector<Codebook> CodebookConfigurations;

            
        } __attribute__((packed));



        // struct 

    } // namespace Vorbis
} // namespace OggMeta

#endif // _OGG_VORBIS_META_H