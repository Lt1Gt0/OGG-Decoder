#include "ogg.h"
#include "oggmeta.h"
#include "endian.h"
#include "Debug/debug.h"
#include "errorhandler.h"
#include "vorbismeta.h"

#include <stdlib.h>

OGG::OGG(char* filepath)
{
    Debug::Print("Attempting to open file");
    this->fp = fopen(filepath, "rb");
    if(this->fp == NULL)
        ErrorHandler::err_n_die("Error opening file...");
}

int OGG::LoadNewPageHeader()
{
    using namespace OggMeta;

    Debug::Print("Loading new page");
    Page page = NULL_PAGE;

    page.Header = new PageHeader;

    // Store the current position of the file in case the capture pattern is invalid
    fpos_t pos;
    fgetpos(fp, &pos);

    fread(page.Header, sizeof(uint8_t), sizeof(PageHeader) - sizeof(uint8_t*), this->fp);

    if (!(Endian::BigEndian32(page.Header->CapturePattern) == (uint32_t)VALID_CAPTURE_PATTERN)) {
        Debug::Print("Invalid Capture Pattern");
        fsetpos(fp, &pos);
        return 0;
    }

    page.Header->SegmentTable = (uint8_t*)malloc(sizeof(uint8_t) * page.Header->PageSegments);
    fread(page.Header->SegmentTable, sizeof(uint8_t), page.Header->PageSegments, this->fp);

    pages.push_back(page);

    if (CheckVorbis()) {
        Debug::Print("Loading Vorbis Application");
    }

    Debug::Print("Loaded Header Page Successfully");
    return 1;
}

int OGG::CheckVorbis()
{
    using namespace OggMeta;

    // Store the current position of the file
    fpos_t pos;
    fgetpos(this->fp, &pos);

    Vorbis::CommonHeader* commonHeader = new Vorbis::CommonHeader;
    fread(commonHeader, sizeof(uint8_t), sizeof(commonHeader), this->fp);

    for (int i = 0; i < 6; i++) {
        // commonHeader is not vorbis application
        if (commonHeader->Magic[i] != VORBIS_OCTET[i]) {
            return 0;
        }
    }

    // Restore file to previous state
    fsetpos(this->fp, &pos);
    delete commonHeader;
    commonHeader = NULL;

    return 1;
}

void OGG::LoadVorbisHeaders()
{
    using namespace OggMeta;

    Vorbis::CommonHeader* common = new Vorbis::CommonHeader;
    fread(common, sizeof(uint8_t), sizeof(common), this->fp);

    Vorbis::IdentificationHeader identification = new Vorbis::IdentificationHeader;
    fread(identification, sizeof(uint8_t), sizeof(Vorbis::IdentificationHeader), this->fp);

    // TODO

}