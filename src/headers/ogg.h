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
        std::vector<OggMeta::Page> mPages;

    public:
        OGG(char* _filepath);

        /**
         * @brief Load a new page into the pages vector
         * 
         * @return (int) status : 0 = success, 1 = failure 
         */
        int LoadNewPageHeader();

    private:
        size_t mFilesize;
        int CheckVorbis();
        void LoadVorbisHeaders();
};

#endif // _OGG_H
