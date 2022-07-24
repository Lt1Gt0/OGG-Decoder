#pragma once
#ifndef _OGG_H
#define _OGG_H

#include <vector>
#include <stdio.h>
#include "oggmeta.h"

class OGG
{ 
    public:
        FILE* mFile;  
        int mCodecType;
        std::vector<OggMeta::Page> mPages;

    private:
        size_t mFilesize;
        OggMeta::CodecEntry mCodecLookup[CODEC_COUNT];
        
    public:
        OGG(char* _filepath);

        /**
         * Load a new page into the pages vector
         * 
         * @return (int) status : 0 = success, 1 = failure 
         */
        int LoadNewPageHeader();

    private:
        /**
         * Determine the application type for a given ogg file and
         * update mApplicationType. If no application types are found
         * then mApplicationType will stay the same as it was before
         * (perferably OggApplications::Unknown)
         */
        void DetermineApplicationType();

        /**
         * Check for a possible a vorbis octet in a filestream 
         */
        int CheckVorbisApplication();

        /**
         * Load packet information for a vorbis application
         * (identification/comments/common)
         */
        int LoadVorbisPacket();
};

#endif // _OGG_H
