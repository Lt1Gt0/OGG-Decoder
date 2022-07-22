#include <stdio.h>
#include "ogg.h"
#include "oggmeta.h"
#include "Debug/debug.h"
#include "Debug/logger.h"
#include "errorhandler.h"

/*
    The current file (test.ogg) does contain a vorbis mapping, currently this program does
    not decode any other mapping formats, more information about this in the README.md
*/

Logger logger;
int main(int argc, char** argv)
{
    if (argc < 2) 
        ErrorHandler::err_n_die("No File Provided\nExiting...");

    // Initialize Logger
    LOG_INIT("logs/", "info")

    char* filepath = argv[1];
    LOG_INFO << "Inputted File: " << filepath << std::endl;

    OGG ogg = OGG(filepath);
    ogg.LoadNewPageHeader();

    return 0;
}
