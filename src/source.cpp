#include "ogg.h"
#include "oggmeta.h"
#include "common.h"
#include "Debug/logger.h"

/*
    The current file (test.ogg) does contain a vorbis mapping, currently this program does
    not decode any other mapping formats, more information about this in the README.md
*/

Logger logger;
int main(int argc, char** argv)
{
    // Initialize Logger
    LOG_INIT("logs/", "info")

    if (argc < 2)
        error(Severity::high, "Usage:", "./ogg_decoder <filepath>");
    
    char* filepath = argv[1];
    OGG ogg = OGG(filepath);

    while (ogg.LoadNewPageHeader() != OggStatus::INVALID_CAPTURE_PATTERN) {
        ogg.mCodecLookup[(int)ogg.mCodecType].afterPHFunc(ogg.mFile); 
    }

    LOG_INFO << "Finished Reading after page header(s)" << std::endl; 

    return 0;
}
