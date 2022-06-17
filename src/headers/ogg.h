#pragma once
#ifndef _OGG_H
#define _OGG_H

#include <stdio.h>
#include <vector>
#include "oggmeta.h"
#include "endian.h"

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
        int LoadNewPage();

    private:
};

#endif // _OGG_H