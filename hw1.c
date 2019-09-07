#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
    char cl_error[] = "ERROR: Invalid arguments\n"
    "USAGE: a.out <regex-file> <input-file>\n";

    if(argc != 3){
        fprintf(stderr, "%s", cl_error);
        return 1;
    }
    printf("no errrors\n");
}