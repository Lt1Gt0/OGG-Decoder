#include <stdio.h>
#include "ogg.h"
#include "oggmeta.h"
#include "Debug/logger.h"

/*
    The current file (test.ogg) does contain a vorbis mapping, currently this program does
    not decode any other mapping formats, more information about this in the README.md
*/

Logger logger;
int main(int argc, char** argv)
{
    if (argc < 2) { 
        printf("Usage: ./ogg_decoder <filepath>\n"); 
        exit(-1);
    }
    
    // Initialize Logger
    LOG_INIT("logs/", "info")

    char* filepath = argv[1];
    LOG_INFO << "Inputted File: " << filepath << std::endl;

    OGG ogg = OGG(filepath);

    while (ogg.LoadNewPageHeader() != INVALID_CAPTURE_PATTERN) {
        ogg.mCodecLookup[(int)ogg.mCodecType].afterPHFunc(ogg.mFile); 
    }

    LOG_INFO << "Finished Reading after page header(s)" << std::endl; 

    return 0;
}
