#include <stdio.h>
#include <stdlib.h>

#include "ogg.h"

int main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stdout, "No file provided\nExiting...\n");
        exit(1);
    }

    char* filepath = argv[1];
    fprintf(stdout, "Inputted file: %s\n", filepath);

    OGG* ogg = new OGG(filepath);        

    return 0;
}