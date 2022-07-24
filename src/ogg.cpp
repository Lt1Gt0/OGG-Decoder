#include "ogg.h"
#include "endian.h"
#include "Debug/logger.h"
#include "oggexceptions.h"

// Codec Includes
#include "vorbis.h"

#include <stdlib.h>

OGG::OGG(char* filepath)
{
    LOG_INFO << "Attempting to open file: " << filepath << std::endl;
    this->mFile = fopen(filepath, "rb");
    
    if (this->mFile == NULL) {
        LOG_ERROR << "Unable to load file: " << this->mFile << std::endl;
        exit(-1);
    }
    
    // Get File size
    fseek(this->mFile, 0, SEEK_END);
    this->mFilesize = ftell(this->mFile);
    rewind(this->mFile);
    LOG_INFO << "File Size: " << this->mFilesize / 1024 << "kB" << std::endl;

    // Initialize Class Members
    this->mCodecType = (int)OggCodec::Unknown;

    this->mCodecLookup[(int)OggCodec::CMML] = {OggMeta::UndefinedCodec};
    this->mCodecLookup[(int)OggCodec::FLAC] = {OggMeta::UndefinedCodec}; 
    this->mCodecLookup[(int)OggCodec::Kate] = {OggMeta::UndefinedCodec};
    this->mCodecLookup[(int)OggCodec::Opus] = {OggMeta::UndefinedCodec};
    this->mCodecLookup[(int)OggCodec::PCM] = {OggMeta::UndefinedCodec};
    this->mCodecLookup[(int)OggCodec::Skeleton] = {OggMeta::UndefinedCodec};
    this->mCodecLookup[(int)OggCodec::Speex] = {OggMeta::UndefinedCodec};
    this->mCodecLookup[(int)OggCodec::Theora] = {OggMeta::UndefinedCodec};
    this->mCodecLookup[(int)OggCodec::Vorbis] = {Vorbis::CheckVorbisCodec};
    this->mCodecLookup[(int)OggCodec::Writ] = {OggMeta::UndefinedCodec};
}

int OGG::LoadNewPageHeader()
{
    using namespace OggMeta;

    LOG_DEBUG << "Loading new page" << std::endl;
    Page page = NULL_PAGE;

    page.Header = new PageHeader;

    // Load everything in the page header excluding the segment table because it is a variable size
    fread(page.Header, sizeof(uint8_t), sizeof(PageHeader) - sizeof(uint8_t*), this->mFile);

    if (Endian::BigEndian32(page.Header->CapturePattern) != (uint32_t)VALID_CAPTURE_PATTERN)
        throw OggException::invalid_capture_pattern;

    // Load the segment table
    page.Header->SegmentTable = new uint8_t[page.Header->PageSegments];
    fread(page.Header->SegmentTable, sizeof(uint8_t), page.Header->PageSegments, this->mFile);

    mPages.push_back(page);

    // Load file information to gather the application type and check for a valid one
    DetermineApplicationType();

    // TODO
    // Not the best way to do this as the application grows to support different application types
    // but for now because I am targeting vorbis applications, just load the packet data 
  
    if (this->mCodecType == (int)OggCodec::Vorbis){
        LOG_DEBUG << "Loading Vorbis Application" << std::endl;
        LoadVorbisPacket();
    }

    // Check for different application types as this program grows

    LOG_SUCCESS << "Loaded Header page" << std::endl;
    return 1;
}

void OggMeta::UndefinedCodec(FILE* fp, int* ret, int codec)
{
    *ret = (int)OggCodec::Unknown;
}

void OGG::DetermineApplicationType()
{
    int codec;
    for (codec = 0; codec < CODEC_COUNT && this->mCodecType == (int)OggCodec::Unknown; codec++) {
        this->mCodecLookup[codec].check(this->mFile, &this->mCodecType, codec);
    }

    if (this->mCodecType == (int)OggCodec::Unknown)
       throw OggException::codec_type_not_found;

    LOG_INFO << "Ogg Application Type: " << (int) --codec << std::endl;
}

int OGG::LoadVorbisPacket()
{
    using namespace OggMeta;
    using namespace Vorbis;

    CommonHeader* commonHeader = new CommonHeader;
    fread(commonHeader, sizeof(uint8_t), sizeof(CommonHeader), this->mFile);

    switch (commonHeader->Packet) {
        case (uint8_t)PacketType::Identification:
        {
            IdentificationHeader* identification = LoadIdentificationHeader(this->mFile);

            if (identification == NULL)
                return INVALID_VORBIS_VERSION; 

            break;
        } 
        case (uint8_t)PacketType::Comment:
        {
            LoadCommentsHeader(this->mFile);
            break;
        } 
        case (uint8_t)PacketType::Setup:
        {
            LoadSetupHeader(this->mFile);
            break;
        }
        default:
        {
            LOG_ERROR << "Invalid Packet Type: " << (int)commonHeader->Packet << std::endl; 
            throw invalid_packet_type; 
        }
    }

    return 1;
}
