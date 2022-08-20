#pragma once
#ifndef _OGG_H
#define _OGG_H

#include <vector>
#include <stdio.h>
#include "oggmeta.h"

enum class OggStatus {
    PAGE_HEADER_SUCCESS,
    PAGE_HEADER_ERROR,

    INVALID_CAPTURE_PATTERN
};

class OGG
{ 
    public:
        FILE* mFile;  
        Codec mCodecType;
        OggMeta::CodecEntry mCodecLookup[CODEC_COUNT];
        std::vector<OggMeta::Page> mPages;

    private:
        size_t mFilesize;
        
    public:
        OGG(const char* _filepath);

        /**
         * Load a new page into the pages vector
         * 
         * @return (int) status : 0 = success, -1 = failure 
         */
        int LoadNewPageHeader();

        /**
         * Determine the application type for a given ogg file and
         * update mApplicationType. If no application types are found
         * then mApplicationType will stay the same as it was before
         * (perferably OggApplications::Unknown)
         */
        void DetermineApplicationType();

        void DumpInfo();
};

#endif // _OGG_H
