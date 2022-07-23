#pragma once
#ifndef _OGG_META_H
#define _OGG_META_H

#include <stdint.h>

#define VALID_CAPTURE_PATTERN 0x4F676753

namespace OggMeta 
{
    #define NULL_PAGE {NULL}

    enum HeaderTypeFlagMask : uint8_t {

    };
    
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
