#pragma once
#ifndef _OGG_VORBIS_META_H
#define _OGG_VORBIS_META_H

#include "oggmeta.h"

#include <stdint.h>

namespace OggMeta {
    namespace Vorbis {
        #define VORBIS_OCTET 0x766F72626973

        struct CommonHeader {
            uint8_t     Packet;
            uint8_t[6]  Magic;            
        };

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
            uint8_t     RESV : 7;
        } __attribute__((packed));

        struct CommentsHeader {

        };

        // struct 

    } // namespace Vorbis
} // namespace OggMeta

namespace Ogg

#endif // _OGG_VORBIS_META_H