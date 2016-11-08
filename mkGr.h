#pragma once
#include <inttypes.h>

/*  strtok_single

    strtok manual implementation where zero string elements are
    taken into account.

    Variables:
        -str = Input string to find next delims.
        -delims = string to separate the input.
    
    Return value:
        Pointer to next start of word, modifiying the delims
        character by '\0' character.
 */
char * strtok_single (char * str, char const * delims);


/*  sep_line

    Given an input string (line) and a string of characters for separation
    (separatos), it separates the line and puts each string in a vector
    of strings (elements). It also computes the number of elements (n).

    Important: The line is useless after this function, so make sure to
    first make a copy if needed.

    Variables:
        -line = Input string to be separated.
        -separatos = Characters to separate the string.
        -elements = Output vector of strings.
        -n = number of elements obtained.
 
 */
void sep_line(char *line, char *separators, char ***elements, uint32_t *n);
