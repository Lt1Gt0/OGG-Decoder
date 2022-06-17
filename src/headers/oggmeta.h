#pragma once
#ifndef _OGG_META_H
#define _OGG_META_H

#include <stdint.h>

#define VALID_CAPTURE_PATTERN 0x4F676753

enum HeaderTypeFlagMask : uint8_t {

};

struct PageHeader {
    uint32_t CapturePattern;
    uint8_t StreamStructureVersion;
};

#endif // _OGG_META_H