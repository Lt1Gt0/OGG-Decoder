#pragma once
#ifndef _OGG_H
#define _OGG_H

#include <vector>
#include <stdio.h>
#include "oggmeta.h"

class OGG
{ 
    public:
        FILE* fp;     
        std::vector<OggMeta::Page> pages;

    public:
        OGG(char* filepath);

        /**
         * @brief Load a new page into the pages vector
         * 
         * @return (int) status : 0 = success, 1 = failure 
         */
        int LoadNewPageHeader();

    private:
        int CheckVorbis();
        void LoadVorbisHeaders();
};

#endif // _OGG_H