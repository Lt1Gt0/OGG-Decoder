#pragma once
#ifndef _OGG_H
#define _OGG_H

#include <stdio.h>
#include "oggmeta.h"

class OGG
{
    public:
        FILE* fp;     

    public:
        OGG(char* filepath);

    private:
        PageHeader* pageHeader;

    private:
        int LoadPageHeader();
        int CheckPageHeader();
};

#endif // _OGG_H