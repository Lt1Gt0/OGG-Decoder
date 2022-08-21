#include "ogg.h"
#include "endian.h"
#include "Debug/logger.h"
#include "common.h"

// Codec Includes
#include "oggmeta.h"
#include "vorbis/vorbis.h"

#include <stdlib.h>

OGG::OGG(const char* filepath)
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
    this->mCodecType = Codec::Unknown;

    this->mCodecLookup[(int)Codec::Dirac] = {Codec::Dirac, UndefinedCodec, NoAfterPageHeader}; 
    this->mCodecLookup[(int)Codec::FLAC] = {Codec::FLAC, UndefinedCodec, NoAfterPageHeader}; 
    this->mCodecLookup[(int)Codec::Skeleton] = {Codec::Skeleton, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)Codec::Writ] = {Codec::Writ, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)Codec::Opus] = {Codec::Opus, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)Codec::Theora] = {Codec::Theora, UndefinedCodec, NoAfterPageHeader};
    //this->mCodecLookup[(int)Codec::Vorbis] = {Codec::Vorbis, Vorbis::CheckCodec, Vorbis::LoadPacket};
    this->mCodecLookup[(int)Codec::Vorbis] = {Codec::Vorbis, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)Codec::Clet] = {Codec::Clet, UndefinedCodec, NoAfterPageHeader}; 
    this->mCodecLookup[(int)Codec::CMML] = {Codec::CMML, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)Codec::JNG] = {Codec::JNG, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)Codec::Kate] = {Codec::Kate, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)Codec::MIDI] = {Codec::MIDI, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)Codec::MNG] = {Codec::MNG, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)Codec::PCM] = {Codec::PCM, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)Codec::PNG] = {Codec::PNG, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)Codec::Speex] = {Codec::Speex, UndefinedCodec, NoAfterPageHeader};
    this->mCodecLookup[(int)Codec::Yuv4Mpeg] = {Codec::Yuv4Mpeg, UndefinedCodec, NoAfterPageHeader};
}

OggStatus OGG::LoadNewPageHeader()
{
    using namespace OggMeta;

    Page page = {};

    // Store current file position incase capture pattern is invalid
    fpos_t prevPos;
    fgetpos(this->mFile, &prevPos);

    // Load everything in the page header excluding the segment table because it is a variable size
    fread(&page.Header, sizeof(uint8_t), sizeof(PageHeader) - sizeof(uint8_t*), this->mFile);

    if (Endian::BigEndian32(page.Header.CapturePattern) != (uint32_t)VALID_CAPTURE_PATTERN) {
        // Restore Previous file position and clean up loaded page header
        fsetpos(this->mFile, &prevPos);
        return OggStatus::INVALID_CAPTURE_PATTERN; 
    }

    LOG_DEBUG << "Loading new page" << std::endl;

    // Load the segment table
    page.Header.SegmentTable = new uint8_t[page.Header.PageSegments];
    fread(&page.Header.SegmentTable, sizeof(uint8_t), page.Header.PageSegments, this->mFile);

    mPages.push_back(page);

    // Load file information to gather the application type and check for a valid one
    DetermineApplicationType();

    LOG_SUCCESS << "Loaded page" << std::endl;
    return OggStatus::PAGE_HEADER_SUCCESS;
}

void OGG::DetermineApplicationType()
{
    for (int codec = 0; codec < CODEC_COUNT && this->mCodecType == Codec::Unknown; codec++) {
        this->mCodecType = this->mCodecLookup[codec].checkFunc(this->mFile, codec);
    }

    // If each of the codecs in the lookup table have been checked and the 
    // ogg codec is still unknown just terminate the program
    if (this->mCodecType == Codec::Unknown)
        error(Severity::high, "Ogg:", "codec type not found -", (int)this->mCodecType);

    LOG_INFO << "Ogg Application Type: " << (int)this->mCodecType << std::endl;
}

/* ---------- HELPER FUNCTIONS ---------- */
Codec OggMeta::UndefinedCodec(FILE* fp, int codec)
{
    return Codec::Unknown;
}

int OggMeta::NoAfterPageHeader(FILE* fp)
{
    // Default to returning 1 as a success code 
    return 1;
}
