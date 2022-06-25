#include "ogg.h"
#include "oggmeta.h"
#include "endian.h"
#include "Debug/debug.h"

#include <stdlib.h>

OGG::OGG(char* filepath)
{
    fprintf(stdout, "Attempting to open file\n");
    this->fp = fopen(filepath, "rb");
    if(this->fp == NULL) {
        fprintf(stderr, "Error opening file...\n");
        exit(1);
    }
}

int OGG::LoadNewPageHeader()
{
    using namespace OggMeta;

    Debug::Print("Loading new page");
    Page page = NULL_PAGE;

    page.Header = new PageHeader;

    // Store the current position of the file in case the capture pattern is invalid
    fpos_t prevPos;
    fgetpos(fp, &prevPos);

    fread(page.Header, sizeof(uint8_t), sizeof(PageHeader) - sizeof(uint8_t*), this->fp);

    if (!(Endian::BigEndian32(page.Header->CapturePattern) == (uint32_t)VALID_CAPTURE_PATTERN)) {
        Debug::Print("Invalid Capture Pattern");
        fsetpos(fp, &prevPos);
        return 0;
    }

    page.Header->SegmentTable = (uint8_t*)malloc(sizeof(uint8_t) * page.Header->PageSegments);
    fread(page.Header->SegmentTable, sizeof(uint8_t), page.Header->PageSegments, this->fp);

    pages.push_back(page);
    

    Debug::Print("Loaded Header Page Successfully");
    return 1;
}