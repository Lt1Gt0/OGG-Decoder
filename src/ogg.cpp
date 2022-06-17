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

    fprintf(stdout, "Loading Page Header Information\n");
    if (!LoadPageHeader()) {
        fprintf(stdout, "Error Loading Page Header Information...\nExiting\n");
        exit(1);
    }

    fprintf(stdout, "Validating Page header information\n");
    if (!CheckPageHeader()) {
        fprintf(stdout, "Invalid Page Header...\nExiting\n");
        exit(1);
    }
}

int OGG::LoadPageHeader()
{
    this->pageHeader = new PageHeader;
    fread(this->pageHeader, sizeof(uint8_t), sizeof(PageHeader), this->fp);
    
    if (!(Endian::BigEndian32(this->pageHeader->CapturePattern) == (uint32_t)VALID_CAPTURE_PATTERN)) {
         fprintf(stdout, "Invalid Capture pattern\n");
         return 0;
     }

    return 1;
}

int OGG::CheckPageHeader()
{
    return 1;
}