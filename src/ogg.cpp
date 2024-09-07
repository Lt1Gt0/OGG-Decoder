#include "ogg.hpp"
#include "endian.hpp"
#include "logger.hpp"
#include "common.hpp"

// Codec Includes
//#include "oggmeta.hpp"
//#include "vorbis/vorbis.hpp"

#include <stdlib.h>


OGG::OGG(std::string _filepath)
{
    //using namespace OggMeta;
    LOG_INFO << "Attempting to open file: " << _filepath << std::endl;
    mFile = fopen(_filepath.c_str(), "rb");
    
    if (mFile == NULL)
        error(Severity::high, "Unable to load file:", mFile);
    
    // Get File size
    fseek(mFile, 0, SEEK_END);
    mFilesize = ftell(mFile);
    rewind(mFile);
    LOG_INFO << "File Size: " << mFilesize / 1024 << "kB" << std::endl;

    // Initialize Class Members
    //mCodecType = Codec::Unknown;

    //mCodecLookup[(int)Codec::Dirac] = {Codec::Dirac, UndefinedCodec, NoAfterPageHeader}; 
    //mCodecLookup[(int)Codec::FLAC] = {Codec::FLAC, UndefinedCodec, NoAfterPageHeader}; 
    //mCodecLookup[(int)Codec::Skeleton] = {Codec::Skeleton, UndefinedCodec, NoAfterPageHeader};
    //mCodecLookup[(int)Codec::Writ] = {Codec::Writ, UndefinedCodec, NoAfterPageHeader};
    //mCodecLookup[(int)Codec::Opus] = {Codec::Opus, UndefinedCodec, NoAfterPageHeader};
    //mCodecLookup[(int)Codec::Theora] = {Codec::Theora, UndefinedCodec, NoAfterPageHeader};
    ////mCodecLookup[(int)Codec::Vorbis] = {Codec::Vorbis, Vorbis::CheckCodec, Vorbis::LoadPacket};
    //mCodecLookup[(int)Codec::Vorbis] = {Codec::Vorbis, UndefinedCodec, NoAfterPageHeader};
    //mCodecLookup[(int)Codec::Clet] = {Codec::Clet, UndefinedCodec, NoAfterPageHeader}; 
    //mCodecLookup[(int)Codec::CMML] = {Codec::CMML, UndefinedCodec, NoAfterPageHeader};
    //mCodecLookup[(int)Codec::JNG] = {Codec::JNG, UndefinedCodec, NoAfterPageHeader};
    //mCodecLookup[(int)Codec::Kate] = {Codec::Kate, UndefinedCodec, NoAfterPageHeader};
    //mCodecLookup[(int)Codec::MIDI] = {Codec::MIDI, UndefinedCodec, NoAfterPageHeader};
    //mCodecLookup[(int)Codec::MNG] = {Codec::MNG, UndefinedCodec, NoAfterPageHeader};
    //mCodecLookup[(int)Codec::PCM] = {Codec::PCM, UndefinedCodec, NoAfterPageHeader};
    //mCodecLookup[(int)Codec::PNG] = {Codec::PNG, UndefinedCodec, NoAfterPageHeader};
    //mCodecLookup[(int)Codec::Speex] = {Codec::Speex, UndefinedCodec, NoAfterPageHeader};
    //mCodecLookup[(int)Codec::Yuv4Mpeg] = {Codec::Yuv4Mpeg, UndefinedCodec, NoAfterPageHeader};
}

//OggStatus OGG::LoadNewPageHeader()
//{
    //using namespace OggMeta;

    //Page page = {};

    //// Store current file position incase capture pattern is invalid
    //fpos_t prevPos;
    //fgetpos(mFile, &prevPos);

    //// Load everything in the page header excluding the segment table because it is a variable size
    //fread(&page.Header, sizeof(uint8_t), sizeof(PageHeader) - sizeof(uint8_t*), mFile);

    //if (Endian::BigEndian32(page.Header.CapturePattern) != (uint32_t)VALID_CAPTURE_PATTERN) {
        //// Restore Previous file position and clean up loaded page header
        //fsetpos(mFile, &prevPos);
        //return OggStatus::INVALID_CAPTURE_PATTERN; 
    //}

    //LOG_DEBUG << "Loading new page" << std::endl;

    //// Load the segment table
    //page.Header.SegmentTable = new uint8_t[page.Header.PageSegments];
    //fread(&page.Header.SegmentTable, sizeof(uint8_t), page.Header.PageSegments, mFile);

    //mPages.push_back(page);

    //// Load file information to gather the application type and check for a valid one
    //DetermineApplicationType();

    //LOG_SUCCESS << "Loaded page" << std::endl;
    //return OggStatus::PAGE_HEADER_SUCCESS;
//}

//void OGG::DetermineApplicationType()
//{
    //for (int codec = 0; codec < CODEC_COUNT && mCodecType == Codec::Unknown; codec++) {
        //mCodecType = mCodecLookup[codec].checkFunc(mFile, codec);
    //}

    //// If each of the codecs in the lookup table have been checked and the 
    //// ogg codec is still unknown just terminate the program
    //if (mCodecType == Codec::Unknown)
        //error(Severity::high, "Ogg:", "codec type not found -", (int)mCodecType);

    //LOG_INFO << "Ogg Application Type: " << (int)mCodecType << std::endl;
//}

//[> ---------- HELPER FUNCTIONS ---------- <]
//Codec OggMeta::UndefinedCodec(FILE* fp, int codec)
//{
    //return Codec::Unknown;
//}

//int OggMeta::NoAfterPageHeader(FILE* fp)
//{
    //// Default to returning 1 as a success code 
    //return 1;
//}
