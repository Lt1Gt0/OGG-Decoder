#include <stdio.h>
#include "ogg.h"
#include "oggmeta.h"
#include "Debug/debug.h"
#include "errorhandler.h"

/*
    The current file (test.ogg) does contain a vorbis mapping, currently this program does
    not decode any other mapping formats, more information about this in the README.md
*/

int main(int argc, char** argv)
{
    if (argc < 2) {
        ErrorHandler::err_n_die("No File Provided\nExiting...");
    }

    char* filepath = argv[1];
    Debug::Print("Inputted file: %s", filepath);

    OGG* ogg = new OGG(filepath);    
    ogg->LoadNewPageHeader();

    return 0;
}
