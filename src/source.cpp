#include <stdio.h>
#include "ogg.h"
#include "oggmeta.h"
#include "Debug/debug.h"
#include "errorhandler.h"

/*
    The current file (test.ogg) dows contain a vorbious application
    which needs to be parsed
*/

/*
Steps to decode (according to Vorbis I Spec)
- Decode packet type flag
- Decode mode number
- Decode window shape (long windows only)
- Decode floor
- Decode residue into residue vectors
- Inverse channel coupling of residue vectors
- Generate floot curve from decoded floor data
- Compute dot product of floor and residue, producing audio spectrum vector
- Inverse monolithic transform of audio spectrum vector, always an MDCT in Vorbis I
- Overlap/add left-hand output of transform with right-hand output of previous frame
- Store right-hand data from transform of current frame for further lapping
- If not the first frame, return results of overlap/add as audio result of current frame
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

    // For a simple sanity check
    uint8_t nextByte;
    fread(&nextByte, 1, 1, ogg->fp);

    return 0;
}