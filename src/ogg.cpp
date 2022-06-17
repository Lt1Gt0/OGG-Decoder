#include "ogg.h"
#include "oggmeta.h"

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
    fread(this->pageHeader, 1, sizeof(PageHeader), this->fp);

    fprintf(stdout, "PageHeader->CapturePattern: %X\n", this->pageHeader->CapturePattern);
    fprintf(stdout, "Valid Capture Pattern: %X\n", VALID_CAPTURE_PATTERN);
    if (!(this->pageHeader->CapturePattern == (uint32_t)VALID_CAPTURE_PATTERN)) {
         fprintf(stdout, "Invalid Capture pattern\n");
         return 0;
     }

    return 1;
}

int OGG::CheckPageHeader()
{
    return 1;
}