#include <stdio.h>
#include "ogg.h"
#include "oggmeta.h"
#include "Debug/debug.h"
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
        
    if (argc < 2) { 
        LOG_ERROR << "No File Provided" << std::endl;
        exit(-1);
    }

    char* filepath = argv[1];
    LOG_INFO << "Inputted File: " << filepath << std::endl;

    OGG ogg = OGG(filepath);
    ogg.LoadNewPageHeader();

    return 0;
}
