#pragma once
#ifndef _OGG_H
#define _OGG_H

#include <vector>
#include <stdio.h>
#include "oggmeta.h"

#define PAGE_HEADER_ERROR       (-1)
#define PAGE_HEADER_SUCCESS     (0)
#define INVALID_CAPTURE_PATTERN (1)

class OGG
{ 
    public:
        FILE* mFile;  
        OggCodec mCodecType;
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
};

#endif // _OGG_H
