#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 256

/*
Recursively determine if this line matches the given regex pattern.
If it does, return 0.
If not, return 1.
line: line being read for a match
reg_pattern: regex pattern 
line_idx: index of the line we are currently looking at
starting_pos: position of the line where this match started (if a char is
not a match, we reset from the idx after this point)
pattern_idx: index in regex pattern we are currently looking at
*/
int match_pattern(const char *line, const char *reg_pattern, 
int line_idx, int starting_pos, int pattern_idx)
{
    // The end of the pattern was reached, so there was a successful match
    if(reg_pattern[pattern_idx] == '\0')
    {
        return starting_pos;
    }
    // The end of the line was reached without finding a match
    if(line[line_idx] == '\n' && reg_pattern[pattern_idx] != '?')
    {
        return -1;
    }
    // Check to see if the current char matches
    if(line[line_idx] == reg_pattern[pattern_idx] || 
    reg_pattern[pattern_idx + 1] == '?') 
    {
        // There is a match, so we can advance both pointers
        return match_pattern(line, reg_pattern, line_idx + 1, starting_pos,
         pattern_idx + 1);
    }
    if(reg_pattern[pattern_idx] == '?')
    {
        return match_pattern(line, reg_pattern, line_idx, starting_pos, 
        pattern_idx + 1);
    }
    // There was not a match, so we reset at starting_pos + 1
    return match_pattern(line, reg_pattern, starting_pos + 1, starting_pos + 1,
    0);
}

void read_regex(const char *re_file, char *reg_pattern)
{
    FILE *regex_file = fopen(re_file, "r");
    if (regex_file == NULL)
    {
        fprintf(stderr, "ERROR: regex file <%s> does not exist\n", re_file);
        exit(1);
    }
    fgets(reg_pattern, sizeof(reg_pattern), regex_file);
    fclose(regex_file);
}

void parse_file(const char *in_file, char *reg_pattern){
    FILE *input_file = fopen(in_file, "r");
    if (input_file == NULL)
    {
        fprintf(stderr, "ERROR: input file <%s> does not exist\n", in_file);
        exit(1);
    }
    char line[MAX_LENGTH];
    while(fgets(line, sizeof(line), input_file))
    {
        if(match_pattern(line, reg_pattern, 0, 0, 0) != -1)
        {
            printf("%s", line);
        }
    }
    fclose(input_file);
}

void perform_grep(char *re_file, char *in_file)
{
    char reg_pattern[MAX_LENGTH];
    read_regex(re_file, reg_pattern);
    parse_file(in_file, reg_pattern);    
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "ERROR: Invalid arguments\n"
                        "USAGE: a.out <regex-file> <input-file>\n");
        return 1;
    }
    perform_grep(argv[1], argv[2]);
}