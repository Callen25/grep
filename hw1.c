#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LENGTH 257

enum meta_char
{
    DOT,
    PLUS,
    STAR,
    QMARK,
    DIGIT,
    NON_DIGIT,
    LETTER,
    NON_LETTER,
    SPACE,
    GROUP,
    COMP_GROUP,
    END
};

typedef struct reg_char
{
    bool is_meta;
    char match_char;
    enum meta_char meta;
    struct reg_char *group;
} reg_char;

bool match_reg(const struct reg_char reg, const char c)
{
    if (reg.is_meta)
    {
        switch (reg.meta)
        {
        case DOT:
            return true;
        case DIGIT:
            return isdigit(c) != 0;
        case NON_DIGIT:
            return isdigit(c) == 0;
        case LETTER:
            return isalpha(c) != 0;
        case NON_LETTER:
            return isalpha(c) == 0;
        case SPACE:
            return c == ' ' || c == '\t';
        case GROUP:
        {
            int i = 0;
            while (reg.group[i].meta != END)
            {
                if (match_reg(reg.group[i], c))
                    return true;
                i++;
            }
            return false;
        }
        case COMP_GROUP:
        {
            int i = 0;
            while (reg.group[i].meta != END)
            {
                if (match_reg(reg.group[i], c))
                    return false;
                i++;
            }
            return true;
        }
        default:
            return false;
        }
    }
    else
    {
        return c == reg.match_char;
    }
}

reg_char *compile_reg(const char *reg_pattern)
{
    reg_char *pattern = malloc(MAX_LENGTH);
    int i = 0, j = 0;

    while (reg_pattern[i] != '\n' && reg_pattern[i] != '\0')
    {
        switch (reg_pattern[i])
        {
        case '.':
        {
            pattern[j].is_meta = true;
            pattern[j].meta = DOT;
        }
        break;
        case '+':
        {
            pattern[j].is_meta = true;
            pattern[j].meta = PLUS;
        }
        break;
        case '*':
        {
            pattern[j].is_meta = true;
            pattern[j].meta = STAR;
        }
        break;
        case '?':
        {
            pattern[j].is_meta = true;
            pattern[j].meta = QMARK;
        }
        break;
        case '\\':
        {
            i += 1;
            switch (reg_pattern[i])
            {
            case 'd':
            {
                pattern[j].is_meta = true;
                pattern[j].meta = DIGIT;
            }
            break;
            case 'D':
            {
                pattern[j].is_meta = true;
                pattern[j].meta = NON_DIGIT;
            }
            break;
            case 'w':
            {
                pattern[j].is_meta = true;
                pattern[j].meta = LETTER;
            }
            break;
            case 'W':
            {
                pattern[j].is_meta = true;
                pattern[j].meta = NON_LETTER;
            }
            break;
            case 's':
            {
                pattern[j].is_meta = true;
                pattern[j].meta = SPACE;
            }
            break;
            default:
            {
                pattern[j].is_meta = false;
                pattern[j].match_char = '\\';
            }
            break;
            }
        }
        break;
        case '[':
        {
            pattern[j].is_meta = true;
            if (reg_pattern[i + 1] == '^')
            {
                pattern[j].meta = COMP_GROUP;
                i++;
            }
            else
            {
                pattern[j].meta = GROUP;
            }
            char reg_group[MAX_LENGTH];
            int k = 0;
            i++;
            while (reg_pattern[i] != ']')
            {
                reg_group[k] = reg_pattern[i];
                i++;
                k++;
            }
            pattern[j].group = compile_reg(reg_group);
        }
        break;
        default:
        {
            pattern[j].is_meta = false;
            pattern[j].match_char = reg_pattern[i];
        }
        break;
        }
        i++;
        j++;
    }
    pattern[j].meta = END;
    return pattern;
}

bool next_match(const char cur, const reg_char *pattern, int pattern_idx)
{
    reg_char cur_reg = pattern[pattern_idx];
    if (cur_reg.meta == END)
        return false;
    else if (match_reg(cur_reg, cur))
        return true;
    else if (pattern[pattern_idx + 1].meta == STAR ||
             pattern[pattern_idx + 1].meta == QMARK)
        return next_match(cur, pattern, pattern_idx + 2);
    else
        return false;
}

int match_p(const char *line, const reg_char *pattern, int line_idx,
            int pattern_idx, int starting_pos)
{
    if (pattern[pattern_idx].meta == END)
        return starting_pos;
    if (line[line_idx] == '\0' || line[line_idx] == '\n')
        return -1;
    else
    {
        reg_char cur = pattern[pattern_idx];
        reg_char next = pattern[pattern_idx + 1];
        if (next.meta == STAR || next.meta == PLUS)
        {
            if (next.meta == PLUS)
            {
                if (!match_reg(cur, line[line_idx]))
                    return match_p(line, pattern, starting_pos + 1, 0,
                                   starting_pos + 1);
            }
            while (match_reg(pattern[pattern_idx], line[line_idx]) &&
                   line[line_idx] != '\0' && line[line_idx] != '\n')
            {
                if (next_match(line[line_idx], pattern, pattern_idx + 2))
                {
                    if (match_p(line, pattern, line_idx, pattern_idx + 2, starting_pos) != -1)
                        return starting_pos;
                }
                line_idx += 1;
            }
            return match_p(line, pattern, line_idx, pattern_idx + 2,
                           starting_pos);
        }
        else if (next.meta == QMARK)
        {
            if (match_reg(cur, line[line_idx]))
            {
                if (match_p(line, pattern, line_idx + 1, pattern_idx + 2,
                            starting_pos) != -1)
                    return starting_pos;
            }
            return match_p(line, pattern, line_idx, pattern_idx + 2,
                           starting_pos);
        }
        else
        {
            if (match_reg(cur, line[line_idx]))
                return match_p(line, pattern, line_idx + 1, pattern_idx + 1,
                               starting_pos);
            return match_p(line, pattern, starting_pos + 1, 0, starting_pos + 1);
        }
    }
}

/* Read regex into string */
reg_char *read_regex(const char *re_file)
{
    FILE *regex_file = fopen(re_file, "r");
    if (regex_file == NULL)
    {
        fprintf(stderr, "ERROR: regex file <%s> does not exist\n", re_file);
        exit(1);
    }
    char reg_pattern[MAX_LENGTH];
    fgets(reg_pattern, MAX_LENGTH, regex_file);
    fclose(regex_file);
    return compile_reg(reg_pattern);
}

/* Read the file while matching each line to regex pattern */
void parse_file(const char *in_file, reg_char *pattern)
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
        if (match_p(line, pattern, 0, 0, 0) != -1)
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
    free(pattern);
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
    parse_file(argv[2], read_regex(argv[1]));
}
