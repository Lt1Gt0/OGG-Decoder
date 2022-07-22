#include "ogg.h"
#include "oggmeta.h"
#include "endian.h"
#include "Debug/debug.h"
#include "Debug/logger.h"
#include "errorhandler.h"
#include "vorbismeta.h"

#include <stdlib.h>

OGG::OGG(char* filepath)
{
    LOG_INFO << "Attempting to open file: " << filepath << std::endl;
    this->mFile = fopen(filepath, "rb");
    
    if (this->mFile == NULL)
        ErrorHandler::err_n_die("Error opening file...");
    
    // Get File size
    fseek(this->mFile, 0, SEEK_END);
    this->mFilesize = ftell(this->mFile);
    rewind(this->mFile);
    LOG_INFO << "File Size: " << this->mFilesize / 1024 << "kB" << std::endl;
}

int OGG::LoadNewPageHeader()
{
    using namespace OggMeta;

    LOG_DEBUG << "Loading new page" << std::endl;
    Page page = NULL_PAGE;

    page.Header = new PageHeader;

    // Store the current position of the file in case the capture pattern is invalid
    fpos_t pos;
    fgetpos(this->mFile, &pos);

    fread(page.Header, sizeof(uint8_t), sizeof(PageHeader) - sizeof(uint8_t*), this->mFile);

    if (!(Endian::BigEndian32(page.Header->CapturePattern) == (uint32_t)VALID_CAPTURE_PATTERN)) {
        LOG_WARN << "Invalid Capture Pattern" << std::endl;
        fsetpos(this->mFile, &pos);
        return 0;
    }

    page.Header->SegmentTable = new uint8_t[page.Header->PageSegments];
    fread(page.Header->SegmentTable, sizeof(uint8_t), page.Header->PageSegments, this->mFile);

    mPages.push_back(page);

    if (CheckVorbis()) {
        LOG_DEBUG << "Loading Vorbis Application" << std::endl;
    }

    // Check for different application types as this program grows

    LOG_SUCCESS << "Loaded Header page" << std::endl;
    return 1;
}

int OGG::CheckVorbis()
{
    using namespace OggMeta;

    // Store the current position of the file
    fpos_t pos;
    fgetpos(this->mFile, &pos);

    Vorbis::CommonHeader* commonHeader = new Vorbis::CommonHeader;
    fread(commonHeader, sizeof(uint8_t), sizeof(Vorbis::CommonHeader), this->mFile);

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
    fread(common, sizeof(uint8_t), sizeof(Vorbis::CommonHeader), this->mFile);

    Vorbis::IdentificationHeader* identification = new Vorbis::IdentificationHeader;
    fread(identification, sizeof(uint8_t), sizeof(Vorbis::IdentificationHeader), this->mFile);

    Vorbis::CommentsHeader* comments = new Vorbis::CommentsHeader;
    fread(comments, sizeof(uint8_t), sizeof(Vorbis::CommentsHeader), this->mFile);
}
