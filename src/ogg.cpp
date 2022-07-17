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
    this->mFile = fopen(filepath, "rb");
    
    if (this->mFile == NULL)
        ErrorHandler::err_n_die("Error opening file...");
    
    // Get File size
    fseek(this->mFile, 0, SEEK_END);
    this->mFilesize = ftell(this->mFile);
    rewind(this->mFile);
    Debug::Print("File Size: %ldKb", this->mFilesize / 1024);
}

int OGG::LoadNewPageHeader()
{
    using namespace OggMeta;

    Debug::Print("Loading new page");
    Page page = NULL_PAGE;

    page.Header = new PageHeader;

    // Store the current position of the file in case the capture pattern is invalid
    fpos_t pos;
    fgetpos(this->mFile, &pos);

    fread(page.Header, sizeof(uint8_t), sizeof(PageHeader) - sizeof(uint8_t*), this->mFile);

    if (!(Endian::BigEndian32(page.Header->CapturePattern) == (uint32_t)VALID_CAPTURE_PATTERN)) {
        Debug::Print("Invalid Capture Pattern");
        fsetpos(this->mFile, &pos);
        return 0;
    }

    page.Header->SegmentTable = new uint8_t[page.Header->PageSegments];
    fread(page.Header->SegmentTable, sizeof(uint8_t), page.Header->PageSegments, this->mFile);

    mPages.push_back(page);

    if (CheckVorbis()) {
        Debug::Print("Loading Vorbis Application");
    }

    // Check for different application types as this program grows

    Debug::Print("Loaded Header Page Successfully");
    return 1;
}

int OGG::CheckVorbis()
{
    using namespace OggMeta;

    // Store the current position of the file
    fpos_t pos;
    fgetpos(this->mFile, &pos);

    Vorbis::CommonHeader* commonHeader = new Vorbis::CommonHeader;
    fread(commonHeader, sizeof(uint8_t), sizeof(commonHeader), this->mFile);

    for (int i = 0; i < 6; i++) {
        // commonHeader is not vorbis application
        if (commonHeader->Magic[i] != VORBIS_OCTET[i]) {
            return 0;
        }
    }

    // Restore file to previous state
    fsetpos(this->mFile, &pos);
    delete commonHeader;
    commonHeader = NULL;

    return 1;
}

void OGG::LoadVorbisHeaders()
{
    using namespace OggMeta;

    Vorbis::CommonHeader* common = new Vorbis::CommonHeader;
    fread(common, sizeof(uint8_t), sizeof(common), this->mFile);

    Vorbis::IdentificationHeader* identification = new Vorbis::IdentificationHeader;
    fread(identification, sizeof(uint8_t), sizeof(Vorbis::IdentificationHeader), this->mFile);

    Vorbis::CommentsHeader* comments = new Vorbis::CommentsHeader;
    fread(comments, sizeof(uint8_t), sizeof(Vorbis::CommentsHeader), this->mFile);
}
