#pragma once
#ifndef _OGG_META_H
#define _OGG_META_H

#include "common.h"
#include <stdint.h>
#include <stdio.h>

/* 
 * TODO - Move this enum into a more appropriate file so I 
 * don't have to look at an header include after declarations
 * again (I swear I know how to code)
 */

/**
 * Different Ogg Codec types that I know exist
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

#include "vorbis/vorbis.h"

#define RAW_PACKET_LOGICAL_DIVIDER  255
#define VALID_CAPTURE_PATTERN       0x4F676753
#define NULL_PAGE                   {NULL}
#define CODEC_COUNT                 10

namespace OggMeta 
{
    /**
     * Check codec function pointer to make easier to quickly check
     * what type of codec a loaded file is
     *
     * @param FILE* - file to read from
     * @param int - codec type from OggCodec enum
     * @return OggCodec - type of codec the function will
     * return with, if the codec is still not known just return
     * OggCodec::Unknown;
     */
    typedef OggCodec (*CheckCodec)(FILE*, int);
    OggCodec UndefinedCodec(FILE* fp, int codec);
   
    /**
     * Allow a codec to run a functions that it needs after loading
     * a page header from a given ogg file
     *
     * @param FILE* - file
     *
     * @return (int) - Status
     */
    typedef int (*AfterPageHeader)(FILE*);
    int NoAfterPageHeader(FILE*);

    struct CodecEntry {
        OggCodec        codec;
        CheckCodec      checkFunc;
        AfterPageHeader afterPHFunc;
    };

    enum class HdrTypeBitFlags : uint8_t {
        PacketType  = 0x01, // unset = fresh packet, set = continued packet
        FirstPage   = 0x02, // unset = first page of logical bitstream, set = first page of logical bitstream (bos) 
        LastPage    = 0x04, // unset, Not last page, set = last page
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
