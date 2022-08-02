#include "ogg.h"
#include "endian.h"
#include "Debug/logger.h"
#include "common.h"

// Codec Includes
#include "vorbis/vorbis.h"

#include <stdlib.h>

OGG::OGG(char* filepath)
{
    using namespace OggMeta;
    LOG_INFO << "Attempting to open file: " << filepath << std::endl;
    this->mFile = fopen(filepath, "rb");
    
    if (this->mFile == NULL)
        error(Severity::high, "Unable to load file:", this->mFile);
    
    // Get File size
    fseek(this->mFile, 0, SEEK_END);
    this->mFilesize = ftell(this->mFile);
    rewind(this->mFile);
    LOG_INFO << "File Size: " << this->mFilesize / 1024 << "kB" << std::endl;

    // Initialize Class Members
    this->mCodecType = OggCodec::Unknown;

    this->mCodecLookup[(int)OggCodec::CMML] = {OggCodec::CMML, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)OggCodec::FLAC] = {OggCodec::FLAC, UndefinedCodec, NoAfterPageHeader}; 
    this->mCodecLookup[(int)OggCodec::Kate] = {OggCodec::Kate, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)OggCodec::Opus] = {OggCodec::Opus, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)OggCodec::PCM] = {OggCodec::PCM, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)OggCodec::Skeleton] = {OggCodec::Skeleton, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)OggCodec::Speex] = {OggCodec::Speex, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)OggCodec::Theora] = {OggCodec::Theora, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)OggCodec::Vorbis] = {OggCodec::Vorbis, Vorbis::CheckCodec, Vorbis::LoadPacket};
    this->mCodecLookup[(int)OggCodec::Writ] = {OggCodec::Writ, UndefinedCodec, NoAfterPageHeader};
}

int OGG::LoadNewPageHeader()
{
    using namespace OggMeta;

    Page page = NULL_PAGE;

    // Store current file position incase capture pattern is invalid
    fpos_t prevPos;
    fgetpos(this->mFile, &prevPos);

    page.Header = new PageHeader;

    // Load everything in the page header excluding the segment table because it is a variable size
    fread(page.Header, sizeof(uint8_t), sizeof(PageHeader) - sizeof(uint8_t*), this->mFile);

    if (Endian::BigEndian32(page.Header->CapturePattern) != (uint32_t)VALID_CAPTURE_PATTERN) {
        // Restore Previous file position and clean up loaded page header
        fsetpos(this->mFile, &prevPos);
        delete page.Header;
        page.Header = NULL;
        return INVALID_CAPTURE_PATTERN; 
    }

    LOG_DEBUG << "Loading new page" << std::endl;

    // Load the segment table
    page.Header->SegmentTable = new uint8_t[page.Header->PageSegments];
    fread(page.Header->SegmentTable, sizeof(uint8_t), page.Header->PageSegments, this->mFile);

    mPages.push_back(page);

    // Load file information to gather the application type and check for a valid one
    DetermineApplicationType();

    LOG_SUCCESS << "Loaded page" << std::endl;
    return PAGE_HEADER_SUCCESS;
}

void OGG::DetermineApplicationType()
{
    for (int codec = 0; codec < CODEC_COUNT && this->mCodecType == OggCodec::Unknown; codec++) {
        this->mCodecType = this->mCodecLookup[codec].checkFunc(this->mFile, codec);
    }

    // If each of the codecs in the lookup table have been checked and the 
    // ogg codec is still unknown just terminate the program
    if (this->mCodecType == OggCodec::Unknown)
        error(Severity::high, "Ogg:", "codec type not found -", (int)this->mCodecType);

    LOG_INFO << "Ogg Application Type: " << (int)this->mCodecType << std::endl;
}

/* ---------- HELPER FUNCTIONS ---------- */
OggCodec OggMeta::UndefinedCodec(FILE* fp, int codec)
{
    return OggCodec::Unknown;
}

int OggMeta::NoAfterPageHeader(FILE* fp)
{
    // Default to returning 1 as a success code 
    return 1;
}
