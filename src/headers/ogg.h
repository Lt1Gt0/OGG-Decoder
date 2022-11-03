#pragma once
#ifndef _OGG_H
#define _OGG_H

#include <vector>
#include <string>
#include <stdio.h>
#include "oggmeta.h"

enum OggStatus : int {
    PAGE_HEADER_SUCCESS     = 0,
    PAGE_HEADER_ERROR       = -1,
    INVALID_CAPTURE_PATTERN = -1,
};

class OGG
{ 
    public:
        OGG(std::string _filepath);

        /**
         * Load a new page into the pages vector
         * 
         * @return (OggStatus) status
         */
        //OggStatus LoadNewPageHeader();

        /**
         * Determine the application type for a given ogg file and
         * update mApplicationType. If no application types are found
         * then mApplicationType will stay the same as it was before
         * (perferably OggApplications::Unknown)
         */
        //void DetermineApplicationType();

        //void DumpInfo();

        FILE* mFile;  
        //Codec mCodecType;
        //OggMeta::CodecEntry mCodecLookup[CODEC_COUNT];
        //std::vector<OggMeta::Page> mPages;

    private:
        size_t mFilesize;
        
};

#endif // _OGG_H
