#include <iostream>
#include <bits/stdc++.h>
// #include "ogg.hpp"
// #include "oggmeta.hpp"
#include "common.hpp"
#include "logger.hpp"
#include "vorbis/bitpacking.hpp"

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
        error(Severity::high, "Usage:", "./ogg-decoder <filepath>");
    
    Vorbis::Bitstream bitstream;
    bitstream.Insert(12);
    bitstream.Insert(-1);

    for (Vorbis::BitPattern pattern : bitstream.mStream) {
        //for (int i = 7; i >= 0; i--) {
        for (int i = 0; i < 7; i++) {
            std::cout << pattern.mBuf[i];
        }
        std::cout << "\n";
    }

    //char* filepath = argv[1];
    //OGG ogg = OGG(filepath);

    //while (ogg.LoadNewPageHeader() != OggStatus::INVALID_CAPTURE_PATTERN) {
        //ogg.mCodecLookup[(int)ogg.mCodecType].afterPHFunc(ogg.mFile); 
    //}

    LOG_INFO << "Finished Reading after page header(s)" << std::endl; 

    return 0;
}
