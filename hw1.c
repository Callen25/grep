#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


#define MAX_LENGTH 258


bool match_slash(const char cur, const char token)
{
    switch (cur)
    {
    case 'd':
    {
        return isdigit(cur) == 1;
    }
    case 'D':
    {
        return isdigit(cur) == 0;
    }
    case 'w':
    {
        return isalpha(cur) == 1;
    }
    case 'W':
    {
        return isalpha(cur) == 0;
    }
    case 's':
    {
        return token == ' ' || token == '\t';
    }
    default:
    {
        return token == '\\';
    }
    }
}

void match_star(const char *line, const char *reg_pattern,
                int line_idx, int pattern_idx)
{
    bool is_slash = false;
    bool is_match = false;
    if (pattern_idx > 0 && reg_pattern[pattern_idx - 1] == '\\')
    {
        is_slash = true;
    }
    if (is_slash)
    {
        is_match = match_slash(line[line_idx], reg_pattern[pattern_idx - 1]);
    }
    else
    {
        is_match = reg_pattern[pattern_idx] == '.' ||
                   reg_pattern[pattern_idx] == line[line_idx];
    }
    while (is_match && line_idx < strlen(line) - 1)
    {
        line_idx++;
        if (is_slash)
        {
            is_match = match_slash(line[line_idx], reg_pattern[pattern_idx - 1]);
        }
        else
        {
            is_match = reg_pattern[pattern_idx] == '.' ||
                       reg_pattern[pattern_idx] == line[line_idx];
        }
    }
}

bool match_char(const char *line, const char *reg_pattern, int line_idx,
                int pattern_idx)
{
    char cur = line[line_idx];
    char token = reg_pattern[pattern_idx];
    if (line_idx > 0)
    {
        if (line[line_idx - 1] == '\\')
        {
            return match_slash(cur, token);
        }
    }
    if (reg_pattern[pattern_idx + 1] == '*')
    {
        match_star(line, reg_pattern, line_idx, pattern_idx);
        return true;
    }
    if (reg_pattern[pattern_idx + 1] == '+')
    {
        if (token == cur || match_slash(cur, token))
        {
            match_star(line, reg_pattern, line_idx, pattern_idx);
            return true;
        }
        return false;
    }
    if (token == '*' || token == '+')
        return true;
    if (token == '.')
        return true;
    if (token == '?')
        return true;
    return token == cur || reg_pattern[pattern_idx + 1] == '?';
}

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
                  int line_idx, int pattern_idx, int starting_pos)
{
    // The end of the pattern was reached, so there was a successful match
    if (reg_pattern[pattern_idx] == '\0')
    {
        return starting_pos;
    }
    // The end of the line was reached without finding a match
    if (line_idx == strlen(line) - 1 && reg_pattern[pattern_idx] != '?')
    {
        return -1;
    }
    // If current char matches, recursively check for next match
    if (match_char(line, reg_pattern, line_idx, pattern_idx))
    {
        return match_pattern(line, reg_pattern, line_idx + 1, pattern_idx + 1,
                             starting_pos);
    }
    else
    {
        return match_pattern(line, reg_pattern, starting_pos + 1, 0,
                             starting_pos + 1);
    }
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

void parse_file(const char *in_file, char *reg_pattern)
{
    FILE *input_file = fopen(in_file, "r");
    if (input_file == NULL)
    {
        fprintf(stderr, "ERROR: input file <%s> does not exist\n", in_file);
        exit(1);
    }
    char line[MAX_LENGTH];
    while (fgets(line, sizeof(line), input_file))
    {
        if (match_pattern(line, reg_pattern, 0, 0, 0) != -1)
        {
            if (line[strlen(line) - 1] == '\n')
            {
                printf("%s", line);
            }
            else
            {
                printf("%s\n", line);
            }
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
