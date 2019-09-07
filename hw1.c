#include <stdio.h>
#include <string.h>


int main(int argc, char **argv)
{
    char cl_error[] = "ERROR: Invalid arguments\n"
    "USAGE: a.out <regex-file> <input-file>\n";

    if(argc != 3) {
        printf("%s", cl_error);
    }
    else {
        printf("Regex File: %s\nInput File: %s\n", argv[1], argv[2]);
    }
}