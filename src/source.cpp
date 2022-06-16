#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stdout, "No file provided\neExiting...\n");
        exit(1);
    }

    char* filepath = argv[1];
    fprintf(stdout, "Inputted file: %s\n", filepath);

    return 0;
}