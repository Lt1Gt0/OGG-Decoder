#include "ogg.h"
#include "vorbis.h"
#include "endian.h"
#include "Debug/debug.h"
#include "Debug/logger.h"
#include "oggexceptions.h"

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
    this->mApplicationType = (int)OggApplications::Unknown;
}

int OGG::LoadNewPageHeader()
{
    using namespace OggMeta;

    LOG_DEBUG << "Loading new page" << std::endl;
    Page page = NULL_PAGE;

    page.Header = new PageHeader;

    fread(page.Header, sizeof(uint8_t), sizeof(PageHeader) - sizeof(uint8_t*), this->mFile);

    if (!(Endian::BigEndian32(page.Header->CapturePattern) == (uint32_t)VALID_CAPTURE_PATTERN))
        throw OggException::invalid_capture_pattern;

    page.Header->SegmentTable = new uint8_t[page.Header->PageSegments];
    fread(page.Header->SegmentTable, sizeof(uint8_t), page.Header->PageSegments, this->mFile);

    mPages.push_back(page);

    // Load file information to gather the application type and check for a valid one
    DetermineApplicationType();
    if (this->mApplicationType == (int)OggApplications::Unknown)
        throw OggException::unknown_application_type;

    // TODO
    // Not the best way to do this as the application grows to support different application types
    // but for now because I am targeting vorbis applications, just load the packet data 
  
    if (this->mApplicationType == (int)OggApplications::OggVorbis){
        LOG_DEBUG << "Loading Vorbis Application" << std::endl;
        LoadVorbisPacket();
    }

    // Check for different application types as this program grows

    LOG_SUCCESS << "Loaded Header page" << std::endl;
    return 1;
}

void OGG::DetermineApplicationType()
{
    // TODO - Work on a better way to determine to correct application
    // type within a ogg file
    CheckVorbisApplication();
}

int OGG::CheckVorbisApplication()
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

    this->mApplicationType = (int)OggApplications::OggVorbis;
    LOG_INFO << "Ogg Application Type: " << (int)this->mApplicationType << std::endl;
    return 1;
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
