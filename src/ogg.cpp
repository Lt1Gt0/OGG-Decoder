#include "ogg.h"
#include "oggmeta.h"
#include "endian.h"

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

int OGG::LoadNewPage()
{
    using namespace OggMeta;

    fprintf(stdout, "Loading new page\n");
    Page page = NULL_PAGE;

    page.Header = new PageHeader;
    fread(page.Header, sizeof(uint8_t), sizeof(PageHeader), this->fp);

    if (!(Endian::BigEndian32(page.Header->CapturePattern) == (uint32_t)VALID_CAPTURE_PATTERN)) {
         fprintf(stdout, "Invalid Capture pattern\n");
         return 0;
    }

    page.Header->SegmentTable = (uint8_t*)malloc(page.Header->PageSegments);
    fread(page.Header->SegmentTable, sizeof(uint8_t), page.Header->PageSegments, this->fp);

    pages.push_back(page);
    
    fprintf(stdout, "Loaded Page Successfully\n");
    return 1;
}