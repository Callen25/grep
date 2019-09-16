#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LENGTH 257

/* see if character matches a meta escape character */
bool match_slash(const char cur, const char token)
{
    switch (token)
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
        return cur == ' ' || cur == '\t';
    }
    default:
    {
        return token == '\\';
    }
    }
}

bool matches_stop(const char *reg_pattern, int pattern_idx, const char cur)
{
    if (reg_pattern[pattern_idx] == '\\')
        return match_slash(cur, reg_pattern[pattern_idx + 1]);
    else
    {
        // If stop char is optional, look for next stop char
        if (reg_pattern[pattern_idx + 1] == '*' ||
            reg_pattern[pattern_idx + 1] == '?')
        {
            if (reg_pattern[pattern_idx + 2] == '\0' ||
                reg_pattern[pattern_idx + 2] == '\n')
                return true;
            return matches_stop(reg_pattern, pattern_idx + 2, cur);
        }
        return cur == reg_pattern[pattern_idx] ||
               reg_pattern[pattern_idx] == '.';
    }
}

/* Used for + and * to match as many chars as possible */
int match_star(const char *line, const char *reg_pattern, int line_idx,
               int pattern_idx, bool is_slash)
{
    // Highest index of char that matches next in regex pattern
    int stop_index = -1;
    char token = reg_pattern[pattern_idx];
    while ((((token == '.' || token == line[line_idx]) && !is_slash) ||
            match_slash(line[line_idx], token)) &&
           line[line_idx] != '\n' && line[line_idx] != '\0')
    {
        // If this matches char after * or + save index
        if (matches_stop(reg_pattern, pattern_idx + 2, line[line_idx]))
            stop_index = line_idx;
        line_idx++;
    }
    return stop_index != -1 ? stop_index : line_idx;
}

/* Determine if current char in line matches with current regex char */
bool match_char(const char *line, const char *reg_pattern, int *line_idx,
                int pattern_idx)
{
    char cur = line[*line_idx];
    char token = reg_pattern[pattern_idx];
    bool is_slash = pattern_idx > 0 && reg_pattern[pattern_idx - 1] == '\\';
    if (token == '\\')
        return true;
    if (reg_pattern[pattern_idx + 1] == '*')
    {
        *line_idx = match_star(line, reg_pattern, *line_idx, pattern_idx,
                               is_slash);
        return true;
    }
    if (reg_pattern[pattern_idx + 1] == '+')
    {
        int new_index = match_star(line, reg_pattern, *line_idx, pattern_idx,
                                   is_slash);
        // If index has not increased, then at least one match was not found
        if (new_index == *line_idx)
            return false;
        *line_idx = new_index;
        return true;
    }
    if (is_slash)
    {
        *line_idx = *line_idx + 1;
        bool res = match_slash(cur, token);
        return res;
    }
    if (reg_pattern[pattern_idx + 1] == '?')
        return true;
    if (token == '*' || token == '+' || token == '?')
        return true;
    if (token == cur || token == '.')
    {
        *line_idx = *line_idx + 1;
        return true;
    }
    return false;
}

/* Recursively match the pattern */
int match_pattern(const char *line, const char *reg_pattern,
                  int line_idx, int pattern_idx, int starting_pos)
{
    // The end of the pattern was reached, so there was a successful match
    if (reg_pattern[pattern_idx] == '\0' || reg_pattern[pattern_idx] == '\n')
    {
        return starting_pos;
    }
    // The end of the line was reached without finding a match
    if (line[line_idx] == '\0' && reg_pattern[pattern_idx] != '?' &&
        reg_pattern[pattern_idx] != '*' && reg_pattern[pattern_idx] != '+')
    {
        return -1;
    }
    // If current char matches, recursively check for next match
    if (match_char(line, reg_pattern, &line_idx, pattern_idx))
    {
        return match_pattern(line, reg_pattern, line_idx, pattern_idx + 1,
                             starting_pos);
    }
    else
    {
        return match_pattern(line, reg_pattern, starting_pos + 1, 0,
                             starting_pos + 1);
    }
}

/* Read regex into string */
void read_regex(const char *re_file, char *reg_pattern)
{
    FILE *regex_file = fopen(re_file, "r");
    if (regex_file == NULL)
    {
        fprintf(stderr, "ERROR: regex file <%s> does not exist\n", re_file);
        exit(1);
    }
    fgets(reg_pattern, MAX_LENGTH, regex_file);
    fclose(regex_file);
}

/* Read the file while matching each line to regex pattern */
void parse_file(const char *in_file, char *reg_pattern)
{
    FILE *input_file = fopen(in_file, "r");
    if (input_file == NULL)
    {
        fprintf(stderr, "ERROR: input file <%s> does not exist\n", in_file);
        exit(1);
    }
    char line[MAX_LENGTH];
    while (fgets(line, MAX_LENGTH, input_file))
    {
        if (match_pattern(line, reg_pattern, 0, 0, 0) != -1)
        {
            // Append newline char if the line does not have one
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

/* Initailize regex pattern and then match */
void perform_grep(char *re_file, char *in_file)
{
    char reg_pattern[MAX_LENGTH];
    read_regex(re_file, reg_pattern);
    parse_file(in_file, reg_pattern);
}

int main(int argc, char **argv)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    if (argc != 3)
    {
        fprintf(stderr, "ERROR: Invalid arguments\n"
                        "USAGE: a.out <regex-file> <input-file>\n");
        return 1;
    }
    perform_grep(argv[1], argv[2]);
}
