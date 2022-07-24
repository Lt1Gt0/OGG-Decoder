#pragma once
#ifndef _OGG_META_H
#define _OGG_META_H

#include "vorbis.h"

#include <stdint.h>
#include <stdio.h>

#define VALID_CAPTURE_PATTERN 0x4F676753
#define NULL_PAGE {NULL}
#define CODEC_COUNT 10

/**
 * Different Ogg Application types that I know exist
 */
enum class OggCodec : int {
    CMML,
    FLAC, // Ogg Mapping
    Kate,
    Opus, // Ogg Mapping
    PCM,
    Skeleton,
    Speex, // Ogg Mapping
    Theora, // Ogg Mapping
    Vorbis, // Ogg Mapping
    Writ,
    Unknown,
};

namespace OggMeta 
{
    /**
     * Check codec function pointer to make easier to quickly check
     * what type of codec a loaded file is
     *
     * @param FILE* - file to read from
     * @param int* - modifiable value/return value (if the file is not
     * the given codec type set return to Codec::Unknown, otherwise return
     * set the value of *ret to value of the codec from the OggCodec enum)
     * @param int - codec type from OggCodec enum
     */
    typedef void (*CheckCodec)(FILE*, int*, int);
    void UndefinedCodec(FILE* fp, int* ret, int codec);

    struct CodecEntry {
        CheckCodec  check;
    };


    enum HeaderTypeFlagMask : uint8_t {

    };
    
    /* 
     * Fields with more than 1 byte length
     * are encoded LSB first
     */
    struct PageHeader {
        uint32_t    CapturePattern;
        uint8_t     StreamStructureVersion;
        uint8_t     HeaderTypeFlag;
        uint64_t    AbsoluteGranulePosition;
        uint32_t    StreamSerialNumber;
        uint32_t    PageSequenceNo;
        uint32_t    PageChecksum;
        uint8_t     PageSegments;
        uint8_t*    SegmentTable;
    } __attribute__((packed));
    
    struct Page {
        PageHeader* Header;
    };
}

#endif // _OGG_META_H
